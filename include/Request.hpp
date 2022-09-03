#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "cctype"
#include "ctime"
#include "stdlib.h"
#include "ServerParser.hpp"

typedef enum {GET, POST, DELETE, UNKNOWN} ReqType;

typedef struct
{
	ReqType	type; 											// Type of the request
	std::string file; 										// File name or path for GET request
	std::map<std::string, std::string> essentialHeaders; 	// Store only useful headers
	std::string body; 										// If there is in the request a body, store it
}		AnalysedRequest;

class   Request {

	public :
				
		/* Constructors & Destructors */
		Request();
		~Request();

		bool append(std::string newReqPart);	// returns true if the request is finished

		/* Accessor */
		int	getCode();

		/* Return Analysed Request */
		AnalysedRequest getAnalysedReq(void);

		/* Create response */
		std::string	createResponse(std::string body, int code = -1);

		static const std::map<int, std::string> statusMsgs; // Documentation: To associate the code to its message
	
	private :

		/* REQUESTS.cpp */
			/* Variables */

		std::string req;
		unsigned int cptLineBody;

		// Contains all the headers of the request with as key: the type and its value: the content
		std::map<std::string, std::string> headers;		
		// Stores the type of the request which will be either GET, POST or DELETE
		ReqType type;
		int code; // Status Code: ex:200 ou 404
		std::string acceptHeader; // Stores the content of the header accept for the content-type part

			/* Functions */
		// Fill in type of the request
		bool	fillTypeReq(std::string line);
		// Parsing error at first line
		bool	errorFirstLine(std::string line);
		// Fill Map Headers
		bool	fillMapHeaders(std::string line);
		// Fill Map body
		bool	fillMapBody(std::string line);
		// Check Accept Header's content
		bool	checkAcceptHeader(void);
		bool	verifyTypeMime(void);

		// Analyze the Request 
		AnalysedRequest analysedReq;
		void interpretReq();
		void analyse();

		void clear();


		/* RESPONSE.cpp */		

		// Fill content type by checking the extension of the file
		std::string	checkFile(std::string file);
		void		printFile(std::string file, std::string ext);
		void		ignoreQ(void);
		std::string manageExt(std::string ext);


		/* UTILS.cpp */
		
		bool isEmpty(std::string line);
		int  stoi(std::string & str);
		
		// Get the date by Wael
		std::string day_of_the_week(std::string dayFormat);
		std::string getDateHeader(void);
		
		// Map status code with message
		static std::map<int, std::string> fillMsgs();

		friend std::ostream& operator<<(std::ostream& os, const Request& req);

};

#endif // REQUEST_HPP
