#include "../include/Server.hpp"
#include "../include/ServerParser.hpp"
#include <pthread.h>

static bool end = false;

void* manageExit(void*)
{
	std::string userInput;

	while (true) {
		std::cin >> userInput;
		if (userInput.find("exit") != std::string::npos) {
			end = true;
			break;
		}
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc > 2) {
		std::cout << "Error : Too much arguments" << std::endl;
		return 1;
	}

	std::string configFileName = (argc == 2) ? argv[1] : "config/default.conf";

	// Initializing servers...
	int n = ServerParser::nbOfServers(configFileName);
	if (n == -1)
		return 2;

	std::vector<Server> servers;
	for (int i = 0; i < n; i++) {
		Server server(configFileName);
		if (!server)
			return 3;
		if (server.in(servers))
			continue;
		servers.push_back(server);
	}

	// Updating servers...
	pthread_t t;
	pthread_create(&t, NULL, manageExit, NULL);

	std::cout << std::endl << "Type \"exit\" to exit" << std::endl << std::endl;

	for (std::vector<Server>::iterator it = servers.begin(); true;
			it = (it == servers.end() - 1) ? servers.begin() : it + 1) {
		it->update();
		if (end)
			break;
	}

	pthread_join(t, NULL);

	return (0);
}
