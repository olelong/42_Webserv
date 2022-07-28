#include "../include/Server.hpp"

Server::Server(std::string configFileName)
{
	(void)configFileName;
	/*if (!this->config.parse(configFileName))
		exit(1);
	this->config.print();*/

	if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cout << "Error: socket creation failed" << std::endl;
		exit(4);
	}

	int on = 1;
	if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		std::cout << "Error: setsockopt failed" << std::endl;
		exit(5);
	}
	this->saddr.sin_family = AF_INET;
	this->saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->saddr.sin_port = htons(8080);

	if (bind(this->sockfd, (struct sockaddr*)&this->saddr, sizeof(this->saddr)) == -1) {
		std::cout << "Error: socket bind failed" << std::endl;
		exit(6);
	}

	if (listen(this->sockfd, BACKLOG) == -1) {
		std::cout << "Error: listen failed" << std::endl;
		exit(7);
	}

	std::cout << "Server succesfully initialized!" << std::endl;
}

void Server::update()
{
	struct pollfd	fds[MAX_CLIENTS] = {{this->sockfd, POLLIN, 0}};
	nfds_t			nfds = 1;

	while (true) {
		int rc = poll(fds, nfds, TIMEOUT);
		if (rc == -1) {
			std::cout << "Error: poll failed" << std::endl;
			return;
		}
		if (rc == 0) // Poll timeout expires
			continue;
		if (rc > 0) { // Some fds have in/out events
			if (fds[0].revents & POLLIN) {	// A client is trying to connect to server
				Client c;
				if ((c.fd = accept(this->sockfd, (struct sockaddr*)&c.addr, &c.size)) == -1) {
					std::cout << "Error: accept failed" << std::endl;
					return;
				}
				this->clients.push_back(c);

				fds[nfds].fd = this->clients.back().fd;
				fds[nfds].events = POLLIN | POLLRDHUP;
				nfds++;

				this->printClient(this->clients.back(), "New client!");
				std::cout << "Number of clients: " << this->clients.size() << std::endl;
			}
			// Handle clients events
			std::string clientRequest;
			for (nfds_t i = 1; i < nfds; i++) {
				for (int mask = 1; fds[i].revents; fds[i].revents &= ~mask, mask <<= 1) {
					switch(fds[i].revents & mask) {
					case POLLIN:	// Data can be read on this client socket
						char buf[BUFFER_SIZE];
						for (int rc; (rc = recv(fds[i].fd, buf, BUFFER_SIZE - 1, MSG_DONTWAIT)) > 0;) {
							buf[rc] = '\0';
							clientRequest.append(buf);
						}
						if (!clientRequest.empty()) {
							this->printClient(this->clients[i - 1], "Request from client");
							std::cout << clientRequest;
							fds[i].events &= ~POLLIN;
							fds[i].events |= POLLOUT;
						}
						break;

					case POLLOUT: // It's now possible to write on client's socket
						std::cout << "Sending response..." << std::endl;
						{
							std::string body = "<!DOCTYPE html>\n<html>\n<title>yes </title>\
												\n<body>\n<div>\n<H1>yes it is connected :) &#129402 </H1>\
												\n</div>\n</br>\n<h2>Download : </h2><a href=\"http://127.0.0.1:8080/blabla.png\">D</a>\
												\n<form action=\"/\" method=\"post\" enctype=\"multipart/form-data\"> \
												  \n<input type=\"text\" name=\"description\" value=\"du texte\"> \
												    \n<input type=\"file\" name=\"monFichier\"><button type=\"submit\">Envoyer</button></form>\
												\n</body>\n</html>";
							char bodySizeStr[21];
							sprintf(bodySizeStr, "%ld", body.size());
							std::string res = std::string("HTTP/1.1 200 OK\nContent-length: ")
												+ bodySizeStr + "\nContent-Type: text/html\n\n"
												+ body;
							if (send(fds[i].fd, res.c_str(), res.size(), 0) == -1) {
								std::cout << "Error: send failed" << std::endl;
								return;
							}
						}
						std::cout << "Response sent :)" << std::endl;
						fds[i].events &= ~POLLOUT;
						fds[i].events |= POLLIN;
						break;

					case POLLRDHUP:	// Client closed the connection
						for (nfds_t j = i; j < nfds - 1; j++)
							fds[j] = fds[j + 1];
						nfds--;
						this->printClient(this->clients[i - 1], "Connection closed by client");
						this->clients.erase(this->clients.begin() + i - 1);
						std::cout << "Number of clients: " << this->clients.size() << std::endl;
						break;
					}
				}
			}
		}
	}
}

void Server::printClient(const Client& client, std::string header)
{
		char str[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &client.addr.sin_addr, str, INET_ADDRSTRLEN);
		std::cout << std::endl;
		std::cout << header;
		std::cout << " {IP: " << str;
		std::cout << ", Port: " << ntohs(client.addr.sin_port) << "}" << std::endl;
}
