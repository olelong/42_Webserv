
#include <iostream>
i#include <sstream>
#iiinclude <string>
#include <map>

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

		/* Accesseur */
		int	getCode();

		/* Return Analyse Request */
		AnalyseRequest getAnalysedReq(void);

		/* Create response */
		std::string	createResponse(std::string body);

	private :
		/* Requests */
				
		// Contiend toute la requete avec comme key le type et sa valeur, le contenu :
		std::map<std::string, std::string> headers;
		// Stocke le type de la requete qui sera soit get, post ou delete :
		ReqType type;
		int code; // code erreur ex:200 ou 404
		static const std::map<int, std::string> statusMsgs; // documentation status code pour mettre la valeur qui va avec le code
		AnalyseRequest analysedReq;
		
		/* Analyse Request */
		AnalysedRequest analyse(void);

		/* Responses */

		// Fill content type by checking the extension of the url
		void	checkExt(std::string url);
		void	fillExt(std::string key, std::string value);


		// Fill the map for the documentation on status code 
		// Const because the data will not change
		// Static because it will be the same for everything for every request
		void	fillStatusMsgs(void);
};
