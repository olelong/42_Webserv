#include "../include/Request.hpp"

/* UTILS */

/* Check if the string is empty or filled with spaces and "^M */
bool Request::isEmpty(std::string line) {
	unsigned int i;
	for (i = 0; isspace(line[i]) || line[i] == 13; i++);
	if (i == line.size())
		return true;
	return false;
}

/* Convert a string into an int */
int Request::stoi(std::string & str) {
	int i;
	std::stringstream(str) >> i;
	return i;
}

/* Fill the map of status code for documentation*/

std::map<int, std::string> Request::fillMsgs() {
	
	std::map<int, std::string> tmp;
	
	tmp.insert(std::pair<int, std::string>(200, "OK"));
	
	// Client
	tmp.insert(std::pair<int, std::string>(400, "Bad Request"));
	tmp.insert(std::pair<int, std::string>(404, "Not Found"));
	tmp.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
	tmp.insert(std::pair<int, std::string>(411, "Length Required"));
	tmp.insert(std::pair<int, std::string>(426, "Upgrade Required"));
	//tmp.insert(std::pair<int, std::string>(401, "Unauthorized"));
	//tmp.insert(std::pair<int, std::string>(403, "Forbidden"));
	//tmp.insert(std::pair<int, std::string>(406, "Not Acceptable"));
	//tmp.insert(std::pair<int, std::string>(408, "Request Timeout"));
	//tmp.insert(std::pair<int, std::string>(409, "Conflict"));
	//tmp.insert(std::pair<int, std::string>(410, "Gone"));
	//tmp.insert(std::pair<int, std::string>(413, "Payload Too Large"));
	//tmp.insert(std::pair<int, std::string>(414, "URI Too Long"));
	//tmp.insert(std::pair<int, std::string>(415, "Unsupported Media Type"));
	//tmp.insert(std::pair<int, std::string>(417, "Expectation Failed"));
	
	//tmp.insert(std::pair<int, std::string>(416, "Range Not Satisfiable"));
	//tmp.insert(std::pair<int, std::string>(429, "Too Many Requests"));
	//tmp.insert(std::pair<int, std::string>(431, "Request Header Fields Too Large"));
	
	return tmp;
}

const std::map<int, std::string> Request::statusMsgs = fillMsgs();

/*   PRINT   */
void	Request::printFillMapHeaders(void) {
	std::cout << "Request: " << std::endl;
	std::cout << "Part Fill Headers of request: " << std::endl;
	for (std::map<std::string,std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++) {
		std::cout << "key: " << it->first << " => ";
		std::cout << "value: " << it->second << std::endl;
	}
}

