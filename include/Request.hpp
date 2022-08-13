
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "cctype"


typedef enum {GET, POST, DELETE, UNKNOWN} ReqType;

typedef struct
{
	ReqType	type; // Type of the request
	std::string file; // File name or path for GET request
	std::map<std::string, std::string> essentialHeaders; // Store only useful headers
	std::string body; // If there is in the request a body, store it
}		AnalysedRequest;

class   Request {

	public :
				
		/* Constructors & Destructors */
		Request(std::string req);
		~Request();

		/* Print type and headers  */
		//void	printFillMapHeaders(Request req);
		void	printFillMapHeaders(void);

		/* Accesseur */
		int	getCode();

		/* Return Analyse Request */
		AnalysedRequest getAnalysedReq(void);

		/* Create response */
		std::string	createResponse(std::string body);

	private :
		/* Requests */
				
		// Contiend toute la requete avec comme key le type et sa valeur, le contenu :
		std::map<std::string, std::string> headers;
		
		// Stocke le type de la requete qui sera soit get, post ou delete :
		ReqType type;
		int code; // code erreur ex:200 ou 404
		std::string acceptHeader; // Stocke le contenu du header Accept pour la partie content-type

		static std::map<int, std::string> fillMsgs();
		static const std::map<int, std::string> statusMsgs; // documentation status code pour mettre la valeur qui va avec le code
		AnalysedRequest analysedReq;
		
		/* Analyse Request */
		void analyse(std::string req);

		/* Responses */

		// Fill content type by checking the extension of the file
		std::string	checkFile(std::string url);
		//void	fillExt(std::string key, std::string value);

		// UTILS
		bool isEmpty(std::string line);
};
