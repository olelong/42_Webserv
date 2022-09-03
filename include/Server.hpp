#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#include "ServerParser.hpp"
#include "Request.hpp"

#define BACKLOG		100	// Number of maximum connexions in queue
#define TIMEOUT		50	// in ms
#define BUFFER_SIZE 1024

class Server
{
public: // Yooyoo don't touch!
	Server(std::string configFileName);
	bool operator!() const;

	void update();

	bool in(const std::vector<Server>& servers);

private:
	ServerParser parser;

	// Socket
	int					sockfd;
	struct sockaddr_in	saddr;

	// Clients
	typedef struct s_Client {
		struct sockaddr_in	addr;
		Request				req;
	} Client;
	std::map<int, Client> clients;

	// Poll fds (fds[0] is server socket, others are clients sockets)
	std::vector<struct pollfd> fds;

	// Clients events management
	void acceptClient();
	void receiveRequest(std::vector<struct pollfd>::iterator clientFd);
	void sendResponse(std::vector<struct pollfd>::iterator clientFd);
	void removeClient(std::vector<struct pollfd>::iterator clientFd);

	// GET, POST, DELETE Methods Functions
	std::string getGETBody(std::string filePath);
	std::string createFile(std::string path, std::string content);
	std::string deleteFile(std::string path);

	/// UTILS ///
	bool parsingSucceed;

	// For Methods Functions
	int code; // if we change request code during its interpretation
	std::string getRequestedFilePath(Request req, ServerParser::Location *loc);
	std::string getDirListPage(std::string dirPath);
	std::string getErrorPage(int code);

	// Cgi
	std::string getCgiOutput(std::string filePath, std::string cgi);
	std::string manageCgiHeaders(std::string res);

	// Debug
	void printError(std::string errorMessage);

};

#endif // SERVER_HPP
