#ifndef SERVERPARSER_HPP
#define SERVERPARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <limits.h>
#include <cstring>
#include <string>
#include <sstream>		//stringstream
#include <algorithm>
#include <fcntl.h>
#include <cctype>		//isdigit	
#include <ctype.h>
#include <string.h>		//strlen
#include <utility>
#include <unistd.h>		//close

#define GRAY	"\033[0;38;5;59m"
#define CYAN	"\033[0;36m"
#define GREEN	"\033[0;38;5;10m"
#define WHITE	"\033[0;39m"
#define YELLOW	"\033[33m"
#define YELLOW2 "\033[0;38;5;100m"
#define PINK	"\033[0;38;5;175m"
#define LIGHTPINK "\033[0;38;5;174m"
#define LIGHTSEAGREEN "\033[0;38;5;37m"
#define HOTPINK	"\033[0;38;5;170m"
#define VIOLET	"\033[0;38;5;177m"
#define MAGENTA "\033[0;35m"
#define	RED		"\033[0;38;5;124m"
#define ORANGE	"\033[0;38;5;202m"
#define	BLUE	"\033[0;38;5;117m"
#define SEAGREEN "\033[0;38;5;78m"
#define PEACH	 "\033[0;38;5;180m"

// CLIENT ERROR
#define	BAD_REQUEST			400
#define	UNAUTHORIZED		401
#define	FORBIDDEN			403
#define NOT_FOUND			404
#define	NOT_ALLOWED			405
#define	GONE				410
#define	PAYLOAD_LARGE		413

// CLINET ERROR DIR
#define	DIR_400				"my_errors/400.html"
#define	DIR_401				"my_errors/401.html"
#define	DIR_403				"my_errors/403.html"
#define	DIR_404				"my_errors/404.html"
#define	DIR_405				"my_errors/405.html"
#define	DIR_410				"my_errors/410.html"
#define	DIR_413				"my_errors/413.html"

// SERVER ERROR
#define	INTERNAL_SERVER		500
#define BAD_GATEWAY			502
#define	SERVICE_UNAVAILABLE	503
#define	GATEWAY_TIMEOUT		504
#define	HTTP_NOTSUPPORT		505

// SERVER ERROR DIR
#define	DIR_500				"my_errors/500.html"
#define	DIR_502				"my_errors/502.html"
#define	DIR_503				"my_errors/503.html"
#define	DIR_504				"my_errors/504.html"
#define	DIR_505				"my_errors/505.html"



class Server;

namespace HttpMethods {
typedef enum { GET, POST, DELETE } type;
}

//utils.cpp
std::vector<std::string> getWords(const std::string& line);

class   ServerParser
{
    public:
	//		ServerParser(std::string configFileName);
	//		ServerParser(ServerParser const &src);
			virtual ~ServerParser(void);
	//		ServerParser  &operator=(ServerParser const &src);

	// Returns number of servers in file passed as parameter
	static int nbOfServers(std::string configFileName);

	// Cela permet d'utiliser le server
	friend class Server;

    private:
	// Methods
    bool parse(std::string configFileName);
    void print();

	bool notInclude(std::string configFileName);

	static int lineNum;
    int portError();

	std::string servers;					// THE server
	
	// Attributes for Server
    int port;   	        				// THE port
	std::string name;						// THE serverName
	std::string root;						// THE root

	unsigned long bodySize;					// bodyclientmax size! only one!
	std::map<int, std::string> errorPages;  // Error_page

	
	// Attributes for Location
	typedef struct {
		std::string dir;
		std::string root;
		std::string upload;						// THE upload
		std::vector<std::string> indexes;
		bool autoIndex;
		std::vector<HttpMethods::type> acceptedMethods;
		std::map<std::string, std::string> cgi;			// cgi path need to be located in location
	} Location;
	std::vector<Location> locations;
	
};

#endif          // SERVERPARSER_HPP
