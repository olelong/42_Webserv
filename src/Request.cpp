#include "../include/Request.hpp"


int Request::statusMsgs = { std::make_pair(200, "OK"),
							std::make_pair(400, "Bad Request") };

/* Constructors & Destructors */

Request::Request(std::string req) : code(200), analyseReq{UNKNOWN, NULL, /*(initmap),*/ NULL}{

	std::stringstream tmp(req);
	std::string line;
	std::getline(tmp, line);
	
	// Fill type of request :
	std::string tmp2;
	for (unsigned int i = 0; line[i] != ' ' && line.size() > 0; i++)
		tmp2.push_back(line[i]);
	std::cout << std::endl << "Part type of request: " << std::endl;
	std::cout << "tmp: " << tmp2 << std::endl;
	std::cout << "line: " << line << std::endl;
	std::cout << "req: " << req << std::endl;
	if (tmp2 == "GET") {
		this->type = GET;
		std::cout << "Youpi we have the type of the request now! 🥳" << std::endl;
	}
	else if (tmp2 == "POST") {
		this->type = POST;
		std::cout << "Youpi we have the type of the request now! 🥳" << std::endl;
	}
	else if (tmp2 == "DELETE") {
		this->type = DELETE;
		std::cout << "Youpi we have the type of the request now! 🥳" << std::endl;
	}
	else {
		this->type = UNKNOWN;
		std::cout << "Request type not found" << std::endl;
		this->code = 400;
	}
	if (this->type)
		this->analysedReq.type = this->type;

	// Fill map headers :
	while (!tmp.eof()) {
		std::getline(tmp, line); // recupere la ligne
		size_t pos = line.find(":");
		if (pos != std::string::npos) // verifie si il y a bien un ":" dans la string
			this->headers.insert(std::pair<std::string, std::string>(line.substr(0, pos), line.substr(pos + 2)));
		else
			this->code = 400;
	}
	std::cout << "Part fill headers of request: " << std::endl;
	for (std::map<std::string,std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
		std::cout << it->first << " -> " << it->second << std::endl;

	// Make analyse of the request
	analyse(req);

}

Request::~Request() {}

/* Accesseurs */

int	Request::getCode() {
	return this->code;
}


/* Analyse Request */
// Faire appel a l'analyse dans le constructeur et dans une autre fonction juste retourner la requete structuree

void Request::analyse(std::string req) {

	if (this->type == GET) {
		std::stringstream tmp(req);
		std::string line;
		std::getline(tmp, line);
		std::string url;

		size_t pos = headers.find("/");
		for (unsigned int i = pos + 1; line[i] != " " && line[i].size() > 0; i++)
			url.push_back(line[i]);
		if (url.size() > 0) // Cas autres pages html ou lien
			analyseReq.file = url;
		if (url.size() = 0) {		// Cas 1er html car rien apres le "/"	
		// file remplir avec url ou essential headders remplir avec url et chaine vide?
			analyseReq.file = "";
		//	analyseReq.essentialHeaders.insert(std::pair<std::string, std::string>("url :", NULL));
		}
		else {
			std::cout << "Error in GET request url;" << std::endl; // trouver code correspondant
		}
	}
	// Rajouter type request : POST, DELETE
	return this->analysedReq;
}

AnalysedRequest Request::getAnalysedRequest(void) {
	return this->analysedReq;
}

/* Create response */
std::string	Request::createResponse(std::string body) {
	int content_length = body.size();

	// Retourner la method: (type de requete GET, POST, DELETE)? NON inutile
	
	// Affiche dans la reponse le status code : ex: "HTTP/1.1 200 OK\n Content-length..."
	std::string response;
	response += this->code + " " + Request::statusMsgs.at(this->code) + "\n".
	
	// Verifier extension de l'url enregistree si c'est un .html, .jpeg etc
	response += "Content-Type: ";
	checkExt(this->analysedReq.file); // or map essential headers
	std::map<std::string, std::string> tmp = this->analysedReq.essentialHeaders.find("content-type");
	if (tmp != this->analysedReq.essentialHeaders.end())
		response += tmp->second + "\n";
	else
		std::cout << "content_type Not Found" << std::endl; // add code et message

	// Affiche dans la reponse la longueur du body:
	response += "Content-Length : " + content_length + "\n";


	// Une fois que les headers sont ajoutes a la reponse
	// Je rajoute a la reponse ensuite le body recu:


	// Est ce que je dois verifier que c est une commande get post ou delete pour savoir si je dois rendre un body? Non wael verifie avant et me renvoie dans ce cas une chaine vide mais verifier dans ce cas si je dois pas renvoyer de content-length ou si il est just emis a 0
	
	response += body;

	// Une fois que la reponse est complete donc elle contient bien le body et les headers
	// wael l'envoie au client avec send()
	

	// DEBUG : Juste pour print la reponse que j'envoie pour etre sure
	for (unsigned int i = 0; i < response.size(); i++)
		std::cout << response[i];
	/*->*/std::cout << response << std::endl;
	std::cout << std::endl;


	return response;
}

void	Request::checkExt(std::string url) {
	// Start from the end and find the first occurence of "."
	size_t pos;
	for (std::string::reverse_iterator rev = url.rbegin(); rev != "." && rev != url.rend(); rev++)
		pos = rev; //pas sure de ce que je recupere
	std::string ext;
	
	// Fill the content of the extension
	for (unsigned int i = pos; url[i]; i++)
		ext = url[i];
	
	// Check which extension it is
	if (ext == "jpeg" || ext == "jpg")
		//this->analysedReq.essentialHeaders.insert(std::pair<std::string, std::string>("content-type: ", "image/jpeg"));
		fillExt("content-type", "image/jpeg");
	else if (ext == "png")
		fillExt("content-type", "image/png");
	else if (ext == "svg")
		fillExt("content-type", "image/svg");
	else if (ext == "html")
		fillExt("content-type", "text/html");
	else if (ext == "css")
		fillExt("content-type", "text/css");
	else if (ext == "json")
		fillExt("content-type", "text/json");
	//else 
		// quoi mettre une extension inconnue ou renvoyer le type grace a l extension
		// et mettre un truc generique avant ?
}

void	Request::fillExt(std::string key, std::string value) {
	this->analysedReq.essentialHeaders.insert(std::pair<std::string, std::string>(key, value));
}


/* Fill the map of status code */
// Verifier les codes qui ne nous sont pas utiles
void	Request::fillStatusMsg(void) {
	this->statusMsgs.insert(std::pair<int, std::string>(100, "Continue"));
	this->statusMsgs.insert(std::pair<int, std::string>(101, "Switching Protocols"));
	this->statusMsgs.insert(std::pair<int, std::string>(103, "Early Hints"));
	this->statusMsgs.insert(std::pair<int, std::string>(200, "OK"));
	this->statusMsgs.insert(std::pair<int, std::string>(201, "Created"));
	this->statusMsgs.insert(std::pair<int, std::string>(202, "Accepted"));
	this->statusMsgs.insert(std::pair<int, std::string>(203, "Non-Authoritative Information"));
	this->statusMsgs.insert(std::pair<int, std::string>(204, "No Content"));
	this->statusMsgs.insert(std::pair<int, std::string>(205, "Reset Content"));
	this->statusMsgs.insert(std::pair<int, std::string>(206, "Partial Content"));
	this->statusMsgs.insert(std::pair<int, std::string>(300, "Moved Permanently"));
	this->statusMsgs.insert(std::pair<int, std::string>(302, "Found"));
	this->statusMsgs.insert(std::pair<int, std::string>(303, "See Other"));
	this->statusMsgs.insert(std::pair<int, std::string>(304, "Not Modified"));
	this->statusMsgs.insert(std::pair<int, std::string>(307, "Temporary Redirect"));
	this->statusMsgs.insert(std::pair<int, std::string>(308, "Permanent Redirect"));
	this->statusMsgs.insert(std::pair<int, std::string>(400, "Bad Request"));
	this->statusMsgs.insert(std::pair<int, std::string>(401, "Unauthorized"));
	this->statusMsgs.insert(std::pair<int, std::string>(402, "Payment Required"));
	this->statusMsgs.insert(std::pair<int, std::string>(403, "Forbidden"));
	this->statusMsgs.insert(std::pair<int, std::string>(404, "Not Found"));
	this->statusMsgs.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
	this->statusMsgs.insert(std::pair<int, std::string>(406, "Not Acceptable"));
	this->statusMsgs.insert(std::pair<int, std::string>(407, "Proxy Authentication Required"));
	this->statusMsgs.insert(std::pair<int, std::string>(408, "Request Timeout"));
	this->statusMsgs.insert(std::pair<int, std::string>(409, "Conflict"));
	this->statusMsgs.insert(std::pair<int, std::string>(410, "Gone"));
	this->statusMsgs.insert(std::pair<int, std::string>(411, "Length Required"));
	this->statusMsgs.insert(std::pair<int, std::string>(412, "Precondition Failed"));
	this->statusMsgs.insert(std::pair<int, std::string>(413, "Payload Too Large"));
	this->statusMsgs.insert(std::pair<int, std::string>(414, "URI Too Long"));
	this->statusMsgs.insert(std::pair<int, std::string>(415, "Unsupported Media Type"));
	this->statusMsgs.insert(std::pair<int, std::string>(416, "Range Not Satisfiable"));
	this->statusMsgs.insert(std::pair<int, std::string>(417, "Expectation Failed"));
	this->statusMsgs.insert(std::pair<int, std::string>(418, "I'm a teapot"));
	this->statusMsgs.insert(std::pair<int, std::string>(422, "Unprocessable Entity"));
	this->statusMsgs.insert(std::pair<int, std::string>(425, "Too Early"));
	this->statusMsgs.insert(std::pair<int, std::string>(426, "Upgrade Required"));
	this->statusMsgs.insert(std::pair<int, std::string>(428, "Precondition Required"));
	this->statusMsgs.insert(std::pair<int, std::string>(429, "Too Many Requests"));
	this->statusMsgs.insert(std::pair<int, std::string>(431, "Request Header Fields Too Large"));
	this->statusMsgs.insert(std::pair<int, std::string>(451, "Unavailable For Legal Reasons"));
	this->statusMsgs.insert(std::pair<int, std::string>(500, "Internal Servor Error"));
	this->statusMsgs.insert(std::pair<int, std::string>(501, "Not Implemented"));
	this->statusMsgs.insert(std::pair<int, std::string>(502, "Bad Gateway"));
	this->statusMsgs.insert(std::pair<int, std::string>(503, "Service Unavailable"));
	this->statusMsgs.insert(std::pair<int, std::string>(504, "Gateway Timeout"));
	this->statusMsgs.insert(std::pair<int, std::string>(505, "HTTP Version Not Supported"));
	this->statusMsgs.insert(std::pair<int, std::string>(506, "Variant Also Negotiates"));
	this->statusMsgs.insert(std::pair<int, std::string>(507, "Insufficient Storage"));
	this->statusMsgs.insert(std::pair<int, std::string>(508, "Loop Detected"));
	this->statusMsgs.insert(std::pair<int, std::string>(510, "Not Extended"));
	this->statusMsgs.insert(std::pair<int, std::string>(511, "Network Authentication Required"));
// manque des status	
}
// => changer et mettre en dehors d une fonction comme en haut
