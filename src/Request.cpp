#include "../include/Request.hpp"


/* Constructors & Destructors */

Request::Request() : cptLineBody(0), type(UNKNOWN), code(200) {}

Request::~Request() {}

void Request::clear() {
	*this = Request();
}

bool Request::append(std::string newReqPart)
{
	static bool oldRet = false;

	if (oldRet) {
		this->clear();
		oldRet = false;
	}

	if (this->code == 501 || this->code == 411)
		return false;

	this->req.append(newReqPart);
	if (this->type == UNKNOWN) {
		std::stringstream ss(this->req);
		std::string firstLine;
		std::getline(ss, firstLine);

		if (this->req.find("\n") == std::string::npos) 
			return false;
		if (!fillTypeReq(firstLine)) 
			return false;
		if (!errorFirstLine(firstLine))
			return false;
	}
	
	if ((this->type == GET || this->type == DELETE) && this->req.find("\r\n\r\n") != std::string::npos) { // GET or DELETE request finished
		this->interpretReq();
		return oldRet = true;
	}

	if (this->type == POST) {
		if (this->req.find("\r\n\r\n") != std::string::npos) {
			size_t chunkPos = this->req.find("Transfert-Encoding:");
			size_t contentLenPos = this->req.find("Content-Length: ");
			size_t bodyStart = this->req.find("\r\n\r\n") + 4;
			if (contentLenPos != std::string::npos) {
				size_t bodyLen = this->req.size() - bodyStart;
				std::string contentLenStr = this->req.substr(contentLenPos + 16);
				size_t contentLen = stoi(contentLenStr);
				if (bodyLen == contentLen) {
					for (size_t i = bodyStart; this->req[i]; i++)
						this->analysedReq.body += this->req[i];
					this->interpretReq();
					return oldRet = true;
				}
				if (bodyLen > contentLen)
					this->code = 411;

			}
			else if (chunkPos != std::string::npos && this->req.find("chunked", chunkPos) != std::string::npos) {
					std::stringstream ss(this->req.substr(bodyStart));
					int dataSize = -1;
					while (!ss.eof()) {
						std::string line;
						std::getline(ss, line);
						bool isFinished = false;
						
						this->cptLineBody++;
						// cptLineBody initialized at 0 not -1
						if (this->cptLineBody % 2 == 1) { // Case: line with size of the data
							// To convert the size in hexa into size_t:
							dataSize = -1;
							if (line.find("\r\n") == std::string::npos) {
								std::stringstream st;
								st << std::hex << line[line.size() - 2]; //retirer \r\n
								st >> dataSize;
							
								// Check if the dataSize = 0
								if (dataSize == 0) {
									isFinished = true;
									this->interpretReq();
									return true;
								}
								else
									isFinished = false;
							}
						}
						else if (this->cptLineBody % 2 == 0) { // Case: line with data

							if (isFinished == false) {
								for(int j = 0; j < dataSize && line[j]; j++)
									this->analysedReq.body += line[j];
							}
						}
					}
				}
				else {
					if (contentLenPos == std::string::npos && chunkPos == std::string::npos) {
						this->code = 411;
						return false;
					}
				}
			}
			else {
				this->code = 411;
				return false;	
			}
		}
		return false;
	}

	/* Accesseurs */

	int	Request::getCode() {
		return this->code;
	} 

	/* Parsing error */

	//Parsing error first line
	bool	Request::errorFirstLine(std::string line) {
			
			// Protocol HTTP/1.1
		std::vector<std::string> words = getWords(line); // split the line

		std::string protocole;
		if (words.size() == 3) {
			// Check Protocol
			protocole = words[2];
			if (protocole != "HTTP/1.1") {
				// Case: This is not the right protocol
				this->code = 505; // HTTP not supported
				return false;
			}
			
			// Fill and check file
			int lenFile = words[1].size();
			if (lenFile == 1 && words[1] == "/") { // Case: of the 1st html because nothing after the "/"
				this->analysedReq.file = words[1];
			}
			else if (lenFile > 0) // Other html pages or link requests than the 1st one
				this->analysedReq.file = words[1];
			else {					  // Case: If there is nothing, no files
				this->code = 400; // Bad Request
				return false;
			}
		}
			// Check nb_arg
		else { // Case: Wrong number of arg
			this->code = 400; // Bad Request
			return false;
		}

		return true;
	}

	// Parsing error in the accept header of the request
	bool	Request::checkAcceptHeader(void) {
		// Accept header 
		if (this->acceptHeader.size() == 0)
			this->acceptHeader += "text/html";
		
		// Check if the "q=*" is in the accept header to ignore it
		ignoreQ();

		// Check the MIME type
		if (!verifyTypeMime())
			return false;

		return true;
	}

	/* Function to ignore the "q=*" in the content of accept */
	void	Request::ignoreQ(void) {
		for (std::string::iterator i = this->acceptHeader.begin();
			i != this->acceptHeader.end(); i++) {
			size_t nb;
			size_t q = this->acceptHeader.find("q=");
			if (q != std::string::npos) {
				nb = 0;
				for (unsigned int j = q; this->acceptHeader[j] != ';' && this->acceptHeader[j] != ',' && j < this->acceptHeader.size(); j++)
					nb++;
				this->acceptHeader.erase(q, nb + 1);
			}
			
			size_t v = this->acceptHeader.find("v=");
			if (v != std::string::npos) {
				nb = 0;
				for (unsigned int j = v; this->acceptHeader[j] != ';' && this->acceptHeader[j] != ',' && j < this->acceptHeader.size(); j++)
					nb++;
				this->acceptHeader.erase(v, nb + 1);
			}
		}
	}

	bool	Request::verifyTypeMime(void) {

		// Verify if there is a slash or a comma at the begin of the line:
		if (this->acceptHeader[0] == ',' || this->acceptHeader[0] == ';' 
				|| this->acceptHeader[0] == '/') {
			this->code = 400; // Bad Request
			return false;
		}
		
		// Check if there is a previous one before
		bool sl = false;

		// Count the number of slashes and commas:
		size_t nbCommas = 0; // Number of comma "," or semicolon ";"
		size_t nbSlashes = 0; // Number of  slashes
		for (unsigned int i = 0; this->acceptHeader[i]; i++) {	// Crosses the line
			if (this->acceptHeader[i] == ',' || this->acceptHeader[i] == ';' ) {  // +1 nbCommas +1 if he finds one
				nbCommas += 1;
				sl = false;
			}
			if (this->acceptHeader[i] == '/' && sl == false ) { // +1 nbSlashes if he finds one
				nbSlashes += 1;
				sl = true;
			}
		}

		// Store the position of the lastest , and ; to compare both and find the
		// the nearest one of the end:
		size_t commas = this->acceptHeader.find_last_of(","); // Position of the last comma","
		if (commas == std::string::npos)
			commas = 0;
		
		size_t pointCom = this->acceptHeader.find_last_of(";"); // Position of the last ";"
		if (pointCom == std::string::npos)
			pointCom = 0;
		
		size_t posCom = 0; // Position of the ; or , the closest to the end
		if (commas > pointCom) // Test which one is closest to the end
			posCom = commas;
		else
			posCom = pointCom;

		// Verify the last word and if there is a slash:
		bool   isSlashInLastWord = false;
		if (posCom > 0) { // If there is one word left after the comma/it is not the end
			for (unsigned int i = posCom + 1; this->acceptHeader[i]; i++) {
				if (this->acceptHeader[i] == '/') {
					nbCommas += 1;
					isSlashInLastWord = true;
				}
			}
			
			if (posCom == this->acceptHeader.size() - 1) // Case: Last ; at position of at end of the line
				isSlashInLastWord = true;

			if (isSlashInLastWord == false) { // Case: Error, did not find a "/" in the last word 
				this->code = 400; // Bad Request
				return false;
			}
		}
		
		// Verify if there is a slash at the end of the line:
		if (this->acceptHeader[this->acceptHeader.size() - 1] == '/') {	// Case: Slash at the end of the line
			this->code = 400; // Bad Request
			return false;
		}
		
		// Case: only one MIME type:
		if (nbSlashes == 1 && nbCommas == 0) { 
			nbSlashes = 0;
			// Count the right number of slashes if there is no commas:
			for (unsigned int i = 0; this->acceptHeader[i]; i++) {	// Crosses the line
				if (this->acceptHeader[i] == '/') // +1 nbSlashes if he finds one
					nbSlashes += 1;
			}
			if (nbSlashes == 1)
				return true;
			else {
				this->code = 400; // Bad Request
				return false;
			}
		}
		
		// Compare the number of slashes with the number of commas:
		if (nbSlashes != nbCommas) { // Case: Not the same number of "," or ";" and "/"
			this->code = 400; // Bad Request
			return false;
		}
		return true;
	}


	/* Analyse */

	AnalysedRequest Request::getAnalysedReq(void) {
		return this->analysedReq;
	}

	/* Fill in the type of request  */
	bool	Request::fillTypeReq(std::string line) {
		std::string type;
		for (unsigned int i = 0; line[i] != ' ' && line.size() > 0 && i < line.size(); i++) {
			type.push_back(line[i]);
		}
		
		// PRINT type steps
		/*std::cout << std::endl << "Part type of request: " << std::endl;
		std::cout << "tmp: " << type << std::endl;
		std::cout << "line: " << line << std::endl;
		*/

		// Fill in the type according to the method
		if (type == "GET")
			this->type = GET;
		else if (type == "POST")
			this->type = POST;
		else if (type == "DELETE")
			this->type = DELETE;
		else {
			this->type = UNKNOWN;
			this->analysedReq.type = this->type;
			this->code = 501; // Not Implemented
			return false;
		}
		if (this->type) {
			this->analysedReq.type = this->type;
		}
		return true;
	}

	/* Fill Map Headers  */
	bool	Request::fillMapHeaders(std::string line) {
		std::stringstream request(this->req);
		std::getline(request, line);
		while (!request.eof()) {
			std::getline(request, line); // Recover the line
			if (isEmpty(line)) // Check if the line is empty, if it contains only spaces or "^M".
				break;
			size_t pos = line.find(":");
			if (pos != std::string::npos && line[pos + 1] == ' ') { // Check if there is a ":" in the line
				
				// Check if there is a similar key in the map to avoid double headers
				std::map<std::string, std::string>::iterator posDouble = this->headers.find(line.substr(0, pos));
				for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
				if (posDouble != this->headers.end()) {
					this->code = 400; // Bad Request 
					return false;
				}

				// Insert headers line by line
				this->headers.insert(std::pair<std::string, std::string>(line.substr(0, pos), line.substr(pos + 2)));
				
				std::string accept = line.substr(0, pos);
				// Stock the content of the header accept 
				if (accept == "Accept")
					this->acceptHeader = line.substr(pos + 2);
			}
			else {
				this->code = 400; // Bad Request
				return false;
			}
		}

		return true;
	}

	/* Fill map body  */
	bool	Request::fillMapBody(std::string line)  {
		std::stringstream request(this->req);
		std::getline(request, line);
		while (!request.eof()) {
			std::getline(request, line); // Recover the line
			if (isEmpty(line)) // Check if the line is empty
				break;
		}
		if (this->type == POST && this->analysedReq.body.size() > 0) {

			// Check if the content-length exists in the headers of the request
			std::map<std::string, std::string>::iterator PoscontLen = this->headers.find("Content-Length");
			std::map<std::string, std::string>::iterator PosTransfertEncod = this->headers.find("Transfert-Encoding");
			if (PoscontLen == this->headers.end() && PosTransfertEncod == this->headers.end()) {
				this->code = 411; // Length Required
				return false;
			}
			/*
			// Fill in the body
			while (!request.eof()) {
				std::getline(request, line);
				this->analysedReq.body += line + "\n";
			}*/

			// why
			//this->analysedReq.body.erase(this->analysedReq.body.size() - 1, 1);
			
			// Case: the content-length is not equal to the body size
			if (PoscontLen != this->headers.end()) {
				size_t contLen = stoi(this->headers.at("Content-Length"));
				if (contLen  != this->analysedReq.body.size()) {
					// Case: invalid Content-Length
					this->code = 411; // Length Required
					return false;
				}
			}
		}
		else if (!request.eof()) { // Case: where there would be an empty line 
								   // between the headers
			while (!request.eof()) {
				std::getline(request, line);
				if (!isEmpty(line)) {
					this->code = 400; // Bad Request
					return false;
				}
			}
		}

		// PRINT BODY
		//std::cout << "BODY2 :" << this->analysedReq.body << std::endl;
		
		return true;
	}


	//Use analysis in the manufacturer 
	//and in another function return the structured query

void Request::interpretReq()
{
	std::stringstream tmp(this->req);
	std::string line;
	std::getline(tmp, line);
	
	// Fill map headers:
	if (!fillMapHeaders(line))
		return ;

	// Fill map body:
	if (!fillMapBody(line))
		return ;

	// Parsing Error(headers):
	if (!checkAcceptHeader())
		return;

	// Do the analysis of the request
	analyse();
}

void Request::analyse() {

	this->analysedReq.type = this->type;
	
	std::stringstream tmp(this->req);
	std::string line;
	std::getline(tmp, line);

	// Fill the file in errorFirstLine()

	// Fill the map headers if there is a url after the first line
	size_t poss = this->req.find("url");
	std::string url;
	if (poss != std::string::npos) { // Check if the "url" header has been found
		for (unsigned int i = poss + 5; this->req[i] != '\n' && this->req[i]; i++) 
			url += this->req[i];
		this->analysedReq.essentialHeaders.insert(std::pair<std::string, std::string>("url", url));
	}
			
	// Fill the map headers if there is an Host
	size_t findHost = this->req.find("Host");
	std::string host;
	if (findHost != std::string::npos) { // Check if the "url" header has been found
		for (unsigned int i = findHost + 5; this->req[i] != '\n' && this->req[i]; i++) 
			host += this->req[i];
		this->analysedReq.essentialHeaders.insert(std::pair<std::string, std::string>("Host", host));
	}
}

std::ostream& operator<<(std::ostream& os, const Request& req)
{
	os << req.req;
	return os;
}

