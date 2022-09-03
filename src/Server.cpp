#include "../include/Server.hpp"

Server::Server(std::string configFileName)
{
	if ((this->parsingSucceed = this->parser.parse(configFileName)) == false)
		return;

	if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		this->printError("socket creation failed");
		return;
	}

	int on = 1;
	if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on)) == -1) {
		this->printError("setsockopt failed");
		return;
	}
	this->saddr.sin_family = AF_INET;
	this->saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->saddr.sin_port = htons(this->parser.port);

	if (bind(this->sockfd, (struct sockaddr*)&this->saddr, sizeof(this->saddr)) == -1) {
		this->printError("socket bind failed");
		return;
	}

	if (listen(this->sockfd, BACKLOG) == -1) {
		this->printError("listen failed");
		return;
	}

	struct pollfd server = {this->sockfd, POLLIN, 0};
	this->fds.push_back(server);

	// Adding server's name to /etc/hosts
	std::string newHost = "127.0.0.1\t" + this->parser.name;
	std::ifstream ihosts("/etc/hosts");
	if (ihosts) {
		std::string line;
		bool exists = false;
		while (!ihosts.eof()) {
			std::getline(ihosts, line);
			if (line == newHost) {
				exists = true;
				break;
			}
		}
		if (!exists) {
			std::ofstream hosts("/etc/hosts", std::ios::out | std::ios::app);
			if (hosts)
				hosts << newHost << std::endl;
		}
	}

	std::cout.width(20);
	std::cout << std::left << this->parser.name;
	std::cout << ": " << this->parser.port << std::endl;
}

bool Server::operator!() const {
	return !this->parsingSucceed;
}

void Server::update()
{
	int rc = poll(this->fds.data(), this->fds.size(), TIMEOUT);

	if (rc == -1) {
		this->printError("poll failed");
		return;
	}
	if (rc > 0) {						// Some fds have in/out events
		// Server event
		if (this->fds[0].revents & POLLIN)
			this->acceptClient();

		// Clients events
		for (std::vector<struct pollfd>::iterator it = this->fds.begin() + 1;
				it != this->fds.end(); it += (it != this->fds.end()) ? 1 : 0) {
			if (it->revents & POLLIN)
				this->receiveRequest(it);
			if (it->revents & POLLOUT)
				this->sendResponse(it);
		}
	}
}

bool Server::in(const std::vector<Server>& servers)
{
	for (std::vector<Server>::const_iterator s = servers.begin(); s != servers.end(); s++)
		if (this->parser.port == s->parser.port) {
			close(this->sockfd);
			return true;
		}
	return false;
}

/// Clients events Management ///
void Server::acceptClient()
{
	int fd;

	Client c;
	socklen_t size = sizeof(c.addr);
	if ((fd = accept(this->sockfd, (struct sockaddr*)&c.addr, &size)) == -1) {
		this->printError("accept failed");
		return;
	}
	this->clients[fd] = c;

	struct pollfd newClient = {fd, POLLIN, 0};
	this->fds.push_back(newClient);
}

void Server::receiveRequest(std::vector<struct pollfd>::iterator clientFd)
{
	char buf[BUFFER_SIZE];

	int rc = recv(clientFd->fd, buf, BUFFER_SIZE, 0);
	if (rc <= 0) {
		this->removeClient(clientFd);
		return;
	}

	if (this->clients[clientFd->fd].req.append(std::string(buf, rc)) || rc < BUFFER_SIZE) {
		// Server is now not listening anymore to client until response is sent
		clientFd->events &= ~POLLIN;
		clientFd->events |= POLLOUT;
	}
}

void Server::sendResponse(std::vector<struct pollfd>::iterator clientFd)
{
	// Get useful variables for the interpretation of the client's request
	Request req = this->clients[clientFd->fd].req;
	AnalysedRequest aReq = req.getAnalysedReq();
	ServerParser::Location* loc = NULL;
	size_t i, max_dir_size = 0, save_i = this->parser.locations.size();
	for (i = 0; i < this->parser.locations.size(); i++)
		if (aReq.file.find(this->parser.locations[i].dir) == 0)
			if (this->parser.locations[i].dir.size() > max_dir_size) {
				max_dir_size = this->parser.locations[i].dir.size();
				save_i = i;
			}
	if (save_i != this->parser.locations.size())
		loc = &this->parser.locations[save_i];

	// Managing some errors...
	this->code = req.getCode();
	if (loc && this->code != 501) {
		if (std::find(loc->acceptedMethods.begin(), loc->acceptedMethods.end(),
					aReq.type) == loc->acceptedMethods.end())
			this->code = 405;
	}
	if (aReq.body.size() > this->parser.bodySize)
		this->code = 413;
	if (aReq.type == POST && (!loc || (loc && loc->upload == "")))
		this->code = 403;

	// do stuff depending on request type
	std::string body;
	std::string file = this->getRequestedFilePath(req, loc);
	if (this->code >= 400 && this->code < 600)
		body = this->getErrorPage(this->code);
	else {
		switch (aReq.type) {
		case GET:
			body = this->getGETBody(file);
			break;
	
		case POST:
			body = this->createFile(file, aReq.body);
			break;

		case DELETE:
			body = this->deleteFile(file);
			break;

		default:
			body = this->getErrorPage(this->code = 501);
			break;
		}
	}

	// Managing CGI...
	std::string* cgi = NULL;
	if (loc && (aReq.type == GET || aReq.type == POST)) {
		for (std::map<std::string, std::string>::iterator it = loc->cgi.begin();
				it != loc->cgi.end(); it++) {
			if (file.find(it->first) == file.size() - it->first.size())
				cgi = &it->second;
		}
		if (cgi)
			body = this->getCgiOutput(file, *cgi);
	}

	// Sending Response!
	std::string res = req.createResponse(body, this->code);

	if (cgi)
		res = this->manageCgiHeaders(res);

	int rc = send(clientFd->fd, res.c_str(), res.size(), 0);
	if (rc <= 0) {
		this->removeClient(clientFd);
		return;
	}

	// Response is sent, server can now start listening to the client again
	clientFd->events &= ~POLLOUT;
	clientFd->events |= POLLIN;
}

void Server::removeClient(std::vector<struct pollfd>::iterator clientFd)
{
	this->clients.erase(clientFd->fd);
	this->fds.erase(clientFd);
}
/// ------------------------- ///

// GET, POST, DELETE Methods Functions
std::string Server::getGETBody(std::string filePath)
{
	if (filePath.find_last_of('/') == filePath.size() - 1)
		return this->getDirListPage(filePath);
	else {
		std::ifstream file(filePath.c_str());
		if (!file)
			return this->getErrorPage(this->code = 404);
		std::stringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}
}

std::string Server::createFile(std::string path, std::string content)
{
	std::string body;
	std::ifstream testFile(path.c_str());
	if (testFile) {
		this->code = 200;
		body = "File successfully updated!";
	}
	else {
		this->code = 201;
		body = "File successfully created!";
	}
	std::ofstream file(path.c_str(), std::ios::out | std::ios::trunc);
	if (!file)
		return this->getErrorPage(this->code = 500);
	std::stringstream ss(content);
	std::string newContent, line;
	while (!ss.eof()) {
		std::getline(ss, line);
		if (line[line.size() == 0 ? 0 : line.size() - 1] == '\r')
			continue;
		newContent += line + '\n';
	}
	file << newContent;
	return body;
}

std::string Server::deleteFile(std::string path)
{
	std::ifstream file(path.c_str());
	if (!file)
		return this->getErrorPage(this->code = 404);
	if (remove(path.c_str()))
		return this->getErrorPage(this->code = 500);
	this->code = 200;
	return "File successfully deleted!";
}

/// UTILS ///
// For Methods Functions
std::string Server::getRequestedFilePath(Request req, ServerParser::Location *loc)
{
	std::string rawFileName = req.getAnalysedReq().file;
	struct stat s;
	if (loc) {
		rawFileName.erase(0, loc->dir.size());
		if (req.getAnalysedReq().type == POST && loc->upload.size() > 1)
			rawFileName.insert(0, loc->upload);
		rawFileName.insert(0, this->parser.root + loc->root);
		bool indexed = false;
		if (!stat(rawFileName.c_str(), &s) && s.st_mode & S_IFDIR) {
			if (rawFileName.find_last_of('/') != rawFileName.size() - 1)
				rawFileName += '/';
			for (std::vector<std::string>::iterator index = loc->indexes.begin();
					index != loc->indexes.end(); index++) {
				std::string filename = rawFileName + *index;
				std::ifstream file(filename.c_str());
				if (file) {
					rawFileName = filename;
					indexed = true;
					break;
				}
			}
			if (!indexed && !loc->autoIndex)
				rawFileName += "index.html";
		}
		else if (rawFileName.find_last_of('/') == rawFileName.size() - 1) {
			std::string withoutEndSlash = rawFileName.substr(0, rawFileName.size() - 1);
			if (!stat(withoutEndSlash.c_str(), &s) && s.st_mode & S_IFREG)
				rawFileName = withoutEndSlash;
		}
	}
	else {
		if (!stat(rawFileName.c_str(), &s) && s.st_mode & S_IFDIR) {
			if (rawFileName.find_last_of('/') != rawFileName.size() - 1)
				rawFileName += '/';
			rawFileName += "index.html";
		}
		rawFileName.insert(0, this->parser.root);
	}
	return rawFileName;
}

std::string Server::getDirListPage(std::string dirPath)
{
	DIR *dir;
	struct dirent *ent;
	std::vector<std::string> files;
	std::string dirName = dirPath;
	dirName.erase(0, this->parser.root.size());

	if ((dir = opendir(dirPath.c_str())) == NULL)
		return this->getErrorPage(this->code = 404);

	while ((ent = readdir(dir)) != NULL) {
		if (!strcmp(ent->d_name, ".") || (!strcmp(ent->d_name, "..") && dirName == "/"))
			continue;
		files.push_back(ent->d_name);
		if (ent->d_type == DT_DIR)
			files.back().push_back('/');
	}
	closedir(dir);
	std::sort(files.begin(), files.end());
	for (std::vector<std::string>::iterator f = files.begin(); f != files.end(); f++)
		if ((*f)[f->size() - 1] == '/')
			for (std::vector<std::string>::iterator f2 = files.begin(); f2 != files.end(); f2++)
				if ((*f2)[f2->size() - 1] != '/') {
					std::iter_swap(f, f2);
					break;
				}
	std::string ret = "<!DOCTYPE html>\n<html>\n<title>I'm tabby</title>\n\
					   \r<H1>Index of " + dirName + "</H1>\n<body>\n\
					   \r<hr style=\"width:100%\", size=\"2\", color=black>\n<p>\n";
	for (std::vector<std::string>::iterator file = files.begin(); file != files.end(); file++)
		ret += "<a href=\"" + dirName + *file + "\">" + *file + "</a>\n<br>\n";
	ret += "</p>\n<hr style=\"width:100%\", size=\"2\", color=black>\n</body>\n</html>";
	return ret;
}

std::string Server::getErrorPage(int code)
{
	if (this->parser.errorPages.find(code) != this->parser.errorPages.end()) {
		std::string errorPagePath = this->parser.root + this->parser.errorPages[code];
		std::ifstream errorPage(errorPagePath.c_str());
		if (errorPage) {
			std::stringstream ss;
			ss << errorPage.rdbuf();
			return ss.str();
		}
	}
	std::stringstream ssCode;
	ssCode << code;
	std::string errorMessage = ssCode.str();
	if (Request::statusMsgs.find(code) != Request::statusMsgs.end())
		errorMessage += std::string(" ") + Request::statusMsgs.at(code);
	std::string errorPage = "<!DOCTYPE html>\n<html>\n<title>" + errorMessage + "</title>\n<body>\n\
							\r<center>\n<H1>" + errorMessage + "</H1>\n</center>\n</body>\n</html>";
	return errorPage;
}

// Cgi
std::string Server::getCgiOutput(std::string filePath, std::string cgi)
{
	int out[2];
	if (pipe(out) == -1)
		return this->getErrorPage(this->code = 500);

	pid_t pid;
	if ((pid = fork()) == -1)
		return this->getErrorPage(this->code = 500);

	if (pid == 0) {
		dup2(out[1], STDOUT_FILENO);
		close(out[0]);
		close(out[1]);
		// Get content length
		std::ifstream file(filePath.c_str());
		if (!file)
			return this->getErrorPage(this->code = 404);
		std::stringstream ss;
		ss << file.rdbuf();
		int contentLen = ss.str().size();
		ss.str("");
		ss << contentLen;
		// Execute cgi
		std::string cgiPath = this->parser.root + cgi;
		char *av[3];
		av[0] = new char[cgi.size() + 1]; strcpy(av[0], cgi.c_str());
		av[1] = new char[filePath.size() + 1]; strcpy(av[1], filePath.c_str());
		av[2] = NULL;
		std::string contentLength = "CONTENT_LENGTH=" + ss.str();
		std::string pathInfo = "PATH_INFO=/" + filePath;
		std::string scriptFilename = "SCRIPT_FILENAME=./" + filePath;
		const char *envc[9] = {
			"GATEWAY_INTERFACE=CGI/1.1", "REQUEST_METHOD=POST",
			"SERVER_PROTOCOL=HTTP/1.1", "REDIRECT_STATUS=200",
			"CONTENT_TYPE=application/x-www-form-urlencoded",
			contentLength.c_str(), pathInfo.c_str(),
			scriptFilename.c_str(), NULL
		};
		char *env[9] = {0};
		for (int i = 0; i < 8; i++) {
			env[i] = new char[strlen(envc[i]) + 1];
			strcpy(env[i], envc[i]);
		}
		close(STDIN_FILENO);
		execve(cgiPath.c_str(), av, env);
		std::cerr << "Fatal error: execve failed" << std::endl;
		exit(1);
	}
	else {
		close(out[1]);
		char buf[BUFFER_SIZE];
		std::string output;
		int rc;
		for (; (rc = read(out[0], buf, BUFFER_SIZE)) > 0;)
			output.append(buf, rc);
		wait(NULL);
		return output;
	}
}

std::string Server::manageCgiHeaders(std::string res)
{
	std::stringstream ss(res);
	std::string line, requestLine;
	int emptyLineCpt = 0, contentLen = 0;
	std::map<std::string, std::string> headers;

	std::getline(ss, requestLine);
	while (!ss.eof()) {
		std::getline(ss, line);
		if (line.empty() || line == "\r")
			emptyLineCpt++;
		if (emptyLineCpt == 2)
			break;
		size_t colons = line.find(':');
		if (colons != std::string::npos) {
			std::string key = line.substr(0, colons);
			for(size_t i = 0; i < key.size(); i++)
				key[i] = tolower(key[i]);
			headers[key] = line.substr(colons + 1);
		}
	}

	std::string newRes = requestLine + '\n';
	for (std::map<std::string, std::string>::iterator h = headers.begin();
			h != headers.end(); h++)
		newRes += h->first + ':' + h->second + '\n';
	newRes += "\r\n";
	while (!ss.eof()) {
		std::getline(ss, line);
		newRes += line + '\n';
		contentLen += line.size() + 1;
	}
	size_t contentLenPos = newRes.find("content-length");
	size_t newLinePos = newRes.find('\n', contentLenPos);
	if (contentLenPos == std::string::npos || newLinePos == std::string::npos)
		return newRes;
	newRes.erase(contentLenPos, newLinePos - contentLenPos);
	std::stringstream ssContentLen;
	ssContentLen << contentLen;
	newRes.insert(contentLenPos, "content-length: " + ssContentLen.str());
	return newRes;
}

// Debug
void Server::printError(std::string errorMessage)
{
	std::cerr << this->parser.name << " error: " << errorMessage << std::endl;
}
