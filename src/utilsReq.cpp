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
	tmp.insert(std::pair<int, std::string>(201, "Created"));
	tmp.insert(std::pair<int, std::string>(204, "No Content"));
	
	// Client
	tmp.insert(std::pair<int, std::string>(400, "Bad Request"));
	tmp.insert(std::pair<int, std::string>(403, "Forbidden"));
	tmp.insert(std::pair<int, std::string>(404, "Not Found"));
	tmp.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
	tmp.insert(std::pair<int, std::string>(411, "Length Required"));
	tmp.insert(std::pair<int, std::string>(413, "Request Entity Too Large"));
	tmp.insert(std::pair<int, std::string>(426, "Upgrade Required"));
	tmp.insert(std::pair<int, std::string>(500, "Internal Server Error"));
	tmp.insert(std::pair<int, std::string>(501, "Not Implemented"));
	tmp.insert(std::pair<int, std::string>(505, "HTTP Version not supported"));
	
	//tmp.insert(std::pair<int, std::string>(401, "Unauthorized"));
	//tmp.insert(std::pair<int, std::string>(406, "Not Acceptable"));
	//tmp.insert(std::pair<int, std::string>(408, "Request Timeout"));
	//tmp.insert(std::pair<int, std::string>(409, "Conflict"));
	//tmp.insert(std::pair<int, std::string>(410, "Gone"));
	//tmp.insert(std::pair<int, std::string>(414, "URI Too Long"));
	//tmp.insert(std::pair<int, std::string>(415, "Unsupported Media Type"));
	//tmp.insert(std::pair<int, std::string>(417, "Expectation Failed"));
	
	//tmp.insert(std::pair<int, std::string>(416, "Range Not Satisfiable"));
	//tmp.insert(std::pair<int, std::string>(429, "Too Many Requests"));
	//tmp.insert(std::pair<int, std::string>(431, "Request Header Fields Too Large"));
	
	return tmp;
}

const std::map<int, std::string> Request::statusMsgs = fillMsgs();


/* TIME */
std::string Request::day_of_the_week(std::string dayFormat)
{
		int month = atoi(dayFormat.substr(0,2).c_str());
		int day = atoi(dayFormat.substr(3,2).c_str());
		int year = atoi(dayFormat.substr(6,4).c_str());

		int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};        
		std::string dates[7]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};      
		for(int i = 0; i < month-1; i++) day += days[i];        
		if ( ( ( year%4 == 0 && year%100 != 0) || year%400 == 0) && month > 2) day++;        
		for(int i = 1971; i < year; i++) {
				if ( ( i%4 == 0 && i%100 != 0) || i%400 == 0) day += 366;
				else day += 365;
		}

		std::string result = dates[ ( day%7 + 3 ) % 7];
		return result;
}

std::string Request::getDateHeader(void)
{
		std::string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		std::time_t t = std::time(0);   // get time now
		std::tm* gmt = std::gmtime(&t);

		std::stringstream date;

		date << ((gmt->tm_mon + 1 < 10) ? "0" : "") << gmt->tm_mon + 1 << "/" << ((gmt->tm_mday < 10) ? "0" : "") << gmt->tm_mday << "/" << gmt->tm_year + 1900;
		std::string weekDay = day_of_the_week(date.str());
		date.str("");

		date << "Date: "
				<< weekDay << ", "
				<< gmt->tm_mday << " "
				<< months[gmt->tm_mon] << ' '
				<< (gmt->tm_year + 1900) << ' '
				<< ((gmt->tm_hour < 10) ? "0" : "") << gmt->tm_hour
				<< ":" << ((gmt->tm_min < 10) ? "0" : "") << gmt->tm_min
				<< ":" << ((gmt->tm_sec < 10) ? "0" : "") << gmt->tm_sec << " "
				<< "GMT";
		return date.str();
}
