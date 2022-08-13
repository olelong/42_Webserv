#include "../include/Request.hpp"


/* Constructors & Destructors */

Request::Request(std::string req) : code(200) {

	std::stringstream tmp(req);
	std::string line;
	std::getline(tmp, line);
	
	// Fill type of request :
	std::string tmp2;
	for (unsigned int i = 0; line[i] != ' ' && line.size() > 0; i++) {
		tmp2.push_back(line[i]);
	}
	
	// PRINT type steps
	std::cout << std::endl << "Part type of request: " << std::endl;
	//std::cout << "tmp: " << tmp2 << std::endl;
	std::cout << "line: " << line << std::endl;
	//std::cout << "req: " << req << std::endl;


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
		std::cout << "PARSING ERROR : Request type not found" << std::endl;
		this->code = 400;
		return ;
	}
	if (this->type)
		this->analysedReq.type = this->type;


	// Parsing Error (first line):
		// Protocole HTTP/1.1
	std::string protocole;
	size_t p_begin = line.find_last_of(" ");
	if (p_begin == std::string::npos) {
		std::cout << "PARSING ERROR : Error in find() in the constructor()" << std::endl;
		return ;
	}
	for (unsigned int i = p_begin + 1; line[i] != ' ' && line[i]; i++) // Checker le protocole HTTP/1.1 pas un autre
		protocole.push_back(line[i]);
	if (protocole != "HTTP/1.1") {
		this->code = 400;
		std::cout << "PARSING ERROR : Protocole is not good, use the HTTP/1.1" << std::endl;
		return ;
	}
		// Check nb_arg
	int	nb_arg = 1; // Pour compter le nombre d'argument dans la 1ere ligne donc doit etre egal a 3
	for (unsigned int i = 0; line[i]; i++) {
		if (line[i] == ' ')
			nb_arg++;
	}
	if (nb_arg != 3) {
		this->code = 400;
		std::cout << "PARSING ERROR : Need three argument in the first line of the request" << std::endl;
		return ;
	}


	// Fill map headers:

	while (!tmp.eof()) {
		std::getline(tmp, line); // recupere la ligne
		//std::cout << "at line : " << line << std::endl;
		if (isEmpty(line)) // Check si la ligne est empty donc ne contient que des espaces ou de "^M"
			break;
		size_t pos = line.find(":");
		//std::cout << "LINE: " << line << std::endl; 
		if (pos != std::string::npos) { // verifie si il y a bien un ":" dans la string
			this->headers.insert(std::pair<std::string, std::string>(line.substr(0, pos), line.substr(pos + 2)));
			std::string temp = line.substr(0, pos);
			//std::cout << "temp: " << temp << std::endl; 
			if (temp == "Accept")
				this->acceptHeader = line.substr(pos + 2);
		}
		else {
			std::cout << "PARSING ERROR : Error find() in constructor() fill map headers part" << std::endl;
			this->code = 400;
			return ;
		}
	}

	// Fill map body:
	if (this->type == POST) {
		while (!tmp.eof()) {
			std::getline(tmp, line);
			this->analysedReq.body += line;
		}
	}
	else if (!tmp.eof()) {
		std::cout << "PARSING ERROR : Headers needs \":\" " << std::endl;
		this->code = 400;
		return ;
	}

	// PRINT BODY
	std::cout << "BODY :" << this->analysedReq.body << std::endl;

	// Parsing Error(headers):
		// Accept header // verifier si il y q bien un accept dans chaque methode de requete!!!!
	if (this->acceptHeader.size() == 0) { // Verifier qu'il y a bien un header Accept dans la requete
		std::cout << "PARSING ERROR : No header Accept in the request" << std::endl;
		this->code = 400;
		return ;
	}
	std::cout << "AcceptHeader :" << this->acceptHeader << std::endl;


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

	this->analysedReq.type = this->type;
	if (this->type == GET) {
		std::stringstream tmp(req);
		std::string line;
		std::getline(tmp, line);
		std::string file;

		// Search the first occurence of "/" in the first line
		size_t pos = line.find("/");
		if (pos == std::string::npos) { // Check if it find "/"
			std::cout << "ERROR : / not Found in analyse()" << std::endl;
			return;
		}


		// Fill file with its path or name
		for (unsigned int i = pos; line[i] != ' ' && line[i]; i++)
			file.push_back(line[i]);
		
		if (file.size() == 1) {		// Cas 1er html car rien apres le "/"	
			this->analysedReq.file = file;
		}
		else if (file.size() > 0) // Cas autres pages html ou lien demandes que la 1ere
			this->analysedReq.file = file;
		else {
			std::cout << "ERROR : in GET request file;" << std::endl; // trouver code correspondant
			return ;
		}


		// Fill map headers si il y a une url apres la premiere ligne
		size_t poss = req.find("url");
		std::string url;
		if (poss != std::string::npos) { // Check if it find "/"
			for (unsigned int i = poss + 5; req[i] != '\n' && req[i]; i++) 
				url += req[i];
			this->analysedReq.essentialHeaders.insert(std::pair<std::string, std::string>("url", url));
		}
	else if (this->type == POST) {
		std::cout << "In POST Method" << std::endl;
	}

	else if (this->type == DELETE) {
		std::cout << "In DELETE Method" << std::endl;

	}
		
		// Print
	/*	std::cout << std::endl;
		std::cout << "Print map in analyse() :" << std::endl;

		for (std::map<std::string,std::string>::iterator it = this->analysedReq.essentialHeaders.begin(); it != this->analysedReq.essentialHeaders.end(); it++) {
			std::cout << "key: " << it->first << " => ";
			std::cout << "value: " << it->second << std::endl;
		}

		std::cout << "end" << std::endl;
	*/
		




	}
	// Rajouter type request : POST, DELETE
}

AnalysedRequest Request::getAnalysedReq(void) {
	return this->analysedReq;
}

/* Create response */
std::string	Request::createResponse(std::string body) {
	
	int content_length = body.size(); 						// Stock the size of the body
	std::string response;
	
	// Affiche dans la reponse le status code : ex: "HTTP/1.1 200 OK\n Content-length..."
	response += "HTTP/1.1 ";
	std::stringstream ss; 									// To convert the code into a string
	ss << this->code;
	response += ss.str() + " ";
	response += Request::statusMsgs.at(this->code) + '\n'; // Add the message code
	
	// Verifier extension du file et le comparer avec le contenu du header Accept
	response += "Content-Type: ";
	response += checkFile(this->analysedReq.file); // or map essential header // fichier ne contient pas force;ent une extension donc pas forcement une erreur donc renvoyer le premier contenu dans acc // fichier ne contient pas force;ent une extension donc pas forcement une erreur donc renvoyer le premier contenu dans acceptepts

	// Affiche dans la reponse la longueur du body:
	// Body string empty if there is not a body
	if (content_length > 0) { // If there is a body

		response += "\nContent-Length : ";
		std::stringstream ss1;
		ss1 << content_length;
		response += ss1.str();

	// Une fois que les headers sont ajoutes a la reponse
	// Je rajoute a la reponse ensuite le body recu:

		response += "\r\n\r\n";
		response += body;
	}

	// Une fois que la reponse est complete donc elle contient bien le body et les headers
	// wael l'envoie au client avec send()
	

	// DEBUG : Just for print the response that i send to Wael
	std::cout << "Response send... : " << std::endl;
	std::cout << response << std::endl;
	std::cout << std::endl;


	return response;
}

// NE FONC>TIONNE PAS SI L EXT EST AU MILIEU OU FIN D ACCEPT
std::string	Request::checkFile(std::string file) {
	std::string contentType;

	// Start from the end and find the first occurence of "."
	size_t point = file.find_last_of(".");
	if (point == std::string::npos) { 			// Case: it doesn't find "."
		if (file == "/" && file.size() == 1) { 	// Case : 1ere page Html demandee par le client
			contentType += "text/html";
			std::cout << "client ask for the 1rst html page" << std::endl;
			return contentType;
		}
		else { // Case: there's no extension, so the content-type is the first accept
			// std::cout << "ERROR : \".\" Not Found" << std::endl; // add code error ?
			size_t virgule = this->acceptHeader.find(',');
			size_t pointVirgule = this->acceptHeader.find(';');
			size_t leGrandVainqueur = pointVirgule;
			if (virgule < pointVirgule)
				leGrandVainqueur = virgule;
			contentType = this->acceptHeader.substr(0, leGrandVainqueur); // fichier ne contient pas force;ent une extension donc pas forcement une erreur donc renvoyer le premier contenu dans accept
			return contentType; // Cense retourne une erreur je pense plutot
		}
	}

	std::string ext;
	// Fill the content of the extension
	for (unsigned int i = point + 1; file[i]; i++)
		ext += file[i];

	// PRINT
	std::cout << "file: " << file << std::endl;
	std::cout << "extension: " << ext << std::endl;
	
	// Search if the extension is in the header Accept
	size_t search = this->acceptHeader.find(ext);
	if (search != std::string::npos) { // case : Find ext in Accept Header
		std::cout << "Accept found!" << std::endl;
		size_t count = 0;
		// on cherche la "," ou ";" precedant notre type MIME pour avoir la position du debut du type MIME
		for (size_t i = search; this->acceptHeader[i] != ',' && this->acceptHeader[i] != ';' && i > 0; i--)
			count++;
		
		// content-type = type MIME correspondant a l'extension du fichier
		int offset = search - count == 0 ? 0 : 1; // on trouve le type mais c'est le premier  du header accept, sinon nous sommes sur une virgule donc il faut incrementer
		for (size_t i = search - count + offset; this->acceptHeader[i] != ',' && this->acceptHeader[i] != ';' && i < this->acceptHeader.size(); i++)
			contentType += this->acceptHeader[i];
		std::cout << this->acceptHeader << std::endl;
	}
	else { // case : it does'nt find "ext"
		// content-type = first type MIME contenu dans accept	
		for (size_t i = 0; this->acceptHeader[i] != ',' && this->acceptHeader[i] != ';' && i < this->acceptHeader.size(); i++)
			contentType += this->acceptHeader[i];
	}
	return contentType;
}

/* Fill the map of status code for documentation*/

std::map<int, std::string> Request::fillMsgs() {
	std::map<int, std::string> tmp;
	tmp.insert(std::pair<int, std::string>(100, "Continue"));
	tmp.insert(std::pair<int, std::string>(101, "Switching Protocols"));
	tmp.insert(std::pair<int, std::string>(103, "Early Hints"));
	tmp.insert(std::pair<int, std::string>(200, "OK"));
	tmp.insert(std::pair<int, std::string>(201, "Created"));
	tmp.insert(std::pair<int, std::string>(202, "Accepted"));
	tmp.insert(std::pair<int, std::string>(203, "Non-Authoritative Information"));
	tmp.insert(std::pair<int, std::string>(204, "No Content"));
	tmp.insert(std::pair<int, std::string>(205, "Reset Content"));
	tmp.insert(std::pair<int, std::string>(206, "Partial Content"));
	tmp.insert(std::pair<int, std::string>(300, "Moved Permanently"));
	tmp.insert(std::pair<int, std::string>(302, "Found"));
	tmp.insert(std::pair<int, std::string>(303, "See Other"));
	tmp.insert(std::pair<int, std::string>(304, "Not Modified"));
	tmp.insert(std::pair<int, std::string>(307, "Temporary Redirect"));
	tmp.insert(std::pair<int, std::string>(308, "Permanent Redirect"));
	tmp.insert(std::pair<int, std::string>(400, "Bad Request"));
	tmp.insert(std::pair<int, std::string>(401, "Unauthorized"));
	tmp.insert(std::pair<int, std::string>(402, "Payment Required"));
	tmp.insert(std::pair<int, std::string>(403, "Forbidden"));
	tmp.insert(std::pair<int, std::string>(404, "Not Found"));
	tmp.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
	tmp.insert(std::pair<int, std::string>(406, "Not Acceptable"));
	tmp.insert(std::pair<int, std::string>(407, "Proxy Authentication Required"));
	tmp.insert(std::pair<int, std::string>(408, "Request Timeout"));
	tmp.insert(std::pair<int, std::string>(409, "Conflict"));
	tmp.insert(std::pair<int, std::string>(410, "Gone"));
	tmp.insert(std::pair<int, std::string>(411, "Length Required"));
	tmp.insert(std::pair<int, std::string>(412, "Precondition Failed"));
	tmp.insert(std::pair<int, std::string>(413, "Payload Too Large"));
	tmp.insert(std::pair<int, std::string>(414, "URI Too Long"));
	tmp.insert(std::pair<int, std::string>(415, "Unsupported Media Type"));
	tmp.insert(std::pair<int, std::string>(416, "Range Not Satisfiable"));
	tmp.insert(std::pair<int, std::string>(417, "Expectation Failed"));
	tmp.insert(std::pair<int, std::string>(418, "I'm a teapot"));
	tmp.insert(std::pair<int, std::string>(422, "Unprocessable Entity"));
	tmp.insert(std::pair<int, std::string>(425, "Too Early"));
	tmp.insert(std::pair<int, std::string>(426, "Upgrade Required"));
	tmp.insert(std::pair<int, std::string>(428, "Precondition Required"));
	tmp.insert(std::pair<int, std::string>(429, "Too Many Requests"));
	tmp.insert(std::pair<int, std::string>(431, "Request Header Fields Too Large"));
	tmp.insert(std::pair<int, std::string>(451, "Unavailable For Legal Reasons"));
	tmp.insert(std::pair<int, std::string>(500, "Internal Servor Error"));
	tmp.insert(std::pair<int, std::string>(501, "Not Implemented"));
	tmp.insert(std::pair<int, std::string>(502, "Bad Gateway"));
	tmp.insert(std::pair<int, std::string>(503, "Service Unavailable"));
	tmp.insert(std::pair<int, std::string>(504, "Gateway Timeout"));
	tmp.insert(std::pair<int, std::string>(505, "HTTP Version Not Supported"));
	tmp.insert(std::pair<int, std::string>(506, "Variant Also Negotiates"));
	tmp.insert(std::pair<int, std::string>(507, "Insufficient Storage"));
	tmp.insert(std::pair<int, std::string>(508, "Loop Detected"));
	tmp.insert(std::pair<int, std::string>(510, "Not Extended"));
	tmp.insert(std::pair<int, std::string>(511, "Network Authentication Required"));
	return tmp;
}

const std::map<int, std::string> Request::statusMsgs = fillMsgs();
// Verifier les codes qui ne nous sont pas utiles


/*   PRINT   */


//void	Request::printFillMapHeaders(Request req) {
void	Request::printFillMapHeaders(void) {
	std::cout << "Request: " << std::endl;
	std::cout << "Part Fill Headers of request: " << std::endl;
	for (std::map<std::string,std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++) {
		std::cout << "key: " << it->first << " => ";
		std::cout << "value: " << it->second << std::endl;
	}
}


/* UTILS */

bool Request::isEmpty(std::string line) {
	unsigned int i;
	for (i = 0; isspace(line[i]) || line[i] == 13; i++);
	if (i == line.size())
		return true;
	return false;
}
