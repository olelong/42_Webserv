#include <ctime>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "../include/Request.hpp"

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

