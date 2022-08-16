
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "cctype"


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
		Request(std::string req);
		~Request();

		/* Print type and headers  */
		void	printFillMapHeaders(void);

		/* Accessor */
		int	getCode();

		/* Return Analysed Request */
		AnalysedRequest getAnalysedReq(void);

		/* Create response */
		std::string	createResponse(std::string body);

	private :

		/* Requests */
			/* Variables */

		// Contains all the headers of the request with as key: the type and its value: the content
		std::map<std::string, std::string> headers;		
		// Stores the type of the request which will be either GET, POST or DELETE
		ReqType type;
		int code; // Status Code: ex:200 ou 404
		std::string acceptHeader; // Stores the content of the header accept for the content-type part

			/* Functions used in the constructor */
		// Fill in type of the request
		bool	fillTypeReq(std::string line);
		// Parsing error at first line
		bool	errorFirstLine(std::string line);
		// Fill Map Headers
		bool	fillMapHeaders(std::string req, std::string line);
		// Fill Map body
		bool	fillMapBody(std::string req, std::string line);
		// Check Accept Header's content
		bool	checkAcceptHeader(void);
		bool	verifyTypeMime(void);

		static std::map<int, std::string> fillMsgs();
		static const std::map<int, std::string> statusMsgs; // Documentation: To associate the code to its message
		AnalysedRequest analysedReq;
		
		/* Analyze the Request */
		void analyse(std::string req);

		/* Responses */

		// Fill content type by checking the extension of the file
		std::string	checkFile(std::string file);
		void		printFile(std::string file, std::string ext);
		void		ignoreQ(void);
		std::string manageExt(std::string ext);

		// UTILS
		bool isEmpty(std::string line);
		int  stoi(std::string & str);
};
