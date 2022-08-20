#include "../include/Request.hpp"


/* Constructors & Destructors */

Request::Request() : code(400) {}

Request::Request(std::string req) : code(200) {

	std::stringstream tmp(req);
	std::string line;
	std::getline(tmp, line);
	
	// Fill in type request :
	if (!fillTypeReq(line))
		return ;

	// Parsing Error (first line):
	if (!errorFirstLine(line))
		return ;

	// Fill map headers:
	if (!fillMapHeaders(req, line))
		return ;

	// Fill map body:
	if (!fillMapBody(req, line))
		return ;

	// Parsing Error(headers):
	if (!checkAcceptHeader())
		return;

	// Do the analysis of the request
	analyse(req);

}

Request::~Request() {}

/* Accesseurs */

int	Request::getCode() {
	return this->code;
} 

/* Parsing error */

//Parsing error first line
bool	Request::errorFirstLine(std::string line) {
		
		// Protocol HTTP/1.1
	std::string protocole;
	size_t p_begin = line.find_last_of(" ");
	if (p_begin == std::string::npos) {
		std::cout << "PARSING ERROR : Error in find() in the constructor()" << std::endl;
		this->code = 400; // Bad Request 
		return false;
	}
	for (unsigned int i = p_begin + 1; isprint(line[i]) && line[i]; i++) // Check the HTTP/1.1 protocol
		protocole.push_back(line[i]);
	if (protocole != "HTTP/1.1") {
		// Case: This is not the right protocol
		this->code = 400; // Bad request 
		std::cout << "PARSING ERROR : Protocole is not good, use the HTTP/1.1" << std::endl;
		return false;
	}
		
		// Check nb_arg
	// La méthode a déjà était vérifiée avant du coup je met nb_arg directement a 1.
	int	nb_arg = 1; // To count the number of arguments in the line so need 3
	for (unsigned int i = 0; line[i]; i++) {
		if (line[i] == ' ')
			nb_arg++;
	}
	if (nb_arg != 3) {
		this->code = 400; // Bad Request
		std::cout << "PARSING ERROR : Need three argument in the first line of the request" << std::endl;
		return false;
	}
	return true;
}

// Parsing error in the accept header of the request
bool	Request::checkAcceptHeader(void) {
	// Accept header 
	std::cout << "acceptHeader.size() = " << this->acceptHeader.size() << std::endl;
	if (this->acceptHeader.size() == 0) {
		//std::cout << "PARSING ERROR : No header Accept in the request" << std::endl;
		//this->code = 400; // Bad Request
		//return false;
		this->acceptHeader += "text/html";
		std::cout << "OK: " << this->acceptHeader << std::endl;
	}
	
	// Check if the "q=*" is in the accept header to ignore it
	ignoreQ();

	// Check the MIME type
	if (!verifyTypeMime())
		return false;

	return true;
}

bool	Request::verifyTypeMime(void) {

	// Verify if there is a slash or a comma at the begin of the line:
	if (this->acceptHeader[0] == ',' || this->acceptHeader[0] == ';' 
			|| this->acceptHeader[0] == '/') {
		this->code = 400; // Bad Request
		std::cout << "PARSING ERROR : Bad type MIME in Header Accept 1" << std::endl;
		return false;
	}
	
	// Check if there is a previous one before
	bool sl = false;

	// Count the number of slashes and commas:
	size_t nbCommas = 0; // Number of comma "," or semicolon ";"
	size_t nbSlashes = 0; // Number of  slashes
	for (unsigned int i = 0; this->acceptHeader[i]; i++) {	// Crosses the line
		if (this->acceptHeader[i] == ',' || this->acceptHeader[i] == ';' ) {  // +1 nbCommas +1 if he finds one
			nbCommas += 1;
			sl = false;
		}
		if (this->acceptHeader[i] == '/' && sl == false ) { // +1 nbSlashes if he finds one
			nbSlashes += 1;
			sl = true;
		}
	}

	// Store the position of the lastest , and ; to compare both and find the
	// the nearest one of the end:
	size_t commas = this->acceptHeader.find_last_of(","); // Position of the last comma","
	if (commas == std::string::npos)
		commas = 0;
	
	size_t pointCom = this->acceptHeader.find_last_of(";"); // Position of the last ";"
	if (pointCom == std::string::npos)
		pointCom = 0;
	
	size_t posCom = 0; // Position of the ; or , the closest to the end
	if (commas > pointCom) // Test which one is closest to the end
		posCom = commas;
	else
		posCom = pointCom;

	// Verify the last word and if there is a slash:
	bool   isSlashInLastWord = false;
	if (posCom > 0) { // If there is one word left after the comma/it is not the end
		for (unsigned int i = posCom + 1; this->acceptHeader[i]; i++) {
			if (this->acceptHeader[i] == '/') {
				nbCommas += 1;
				isSlashInLastWord = true;
			}
		}
		
		if (posCom == this->acceptHeader.size() - 1) // Case: Last ; at position of at end of the line
			isSlashInLastWord = true;

		if (isSlashInLastWord == false) { // Case: Error, did not find a "/" in the last word 
			this->code = 400; // Bad Request
			std::cout << "PARSING ERROR : Bad type MIME in Header Accept 2" << std::endl;
			return false;
		}
	}
	
	// Verify if there is a slash at the end of the line:
	if (this->acceptHeader[this->acceptHeader.size() - 1] == '/') {	// Case: Slash at the end of the line
		this->code = 400; // Bad Request
		std::cout << "PARSING ERROR : Bad type MIME in Header Accept 3" << std::endl;
		return false;
	}
	
	// Case: only one MIME type:
	if (nbSlashes == 1 && nbCommas == 0) { 
		nbSlashes = 0;
		// Count the right number of slashes if there is no commas:
		for (unsigned int i = 0; this->acceptHeader[i]; i++) {	// Crosses the line
			if (this->acceptHeader[i] == '/') // +1 nbSlashes if he finds one
				nbSlashes += 1;
		}
		if (nbSlashes == 1)
			return true;
		else {
			this->code = 400; // Bad Request
			std::cout << "PARSING ERROR : Bad type MIME in Header Accept 4" << std::endl;
			return false;
		}
	}
	
	// Compare the number of slashes with the number of commas:
	if (nbSlashes != nbCommas) { // Case: Not the same number of "," or ";" and "/"
		this->code = 400; // Bad Request
		std::cout << "PARSING ERROR : Bad type MIME in Header Accept 5" << std::endl;
		return false;
	}
	return true;
}


/* Analyse */

AnalysedRequest Request::getAnalysedReq(void) {
	return this->analysedReq;
}

/* Fill in the type of request  */
bool	Request::fillTypeReq(std::string line) {
	std::string type;
	for (unsigned int i = 0; line[i] != ' ' && line.size() > 0; i++) {
		type.push_back(line[i]);
	}
	
	// PRINT type steps
	/*std::cout << std::endl << "Part type of request: " << std::endl;
	std::cout << "tmp: " << type << std::endl;
	std::cout << "line: " << line << std::endl;*/

	// Fill in the type according to the method
	if (type == "GET")
		this->type = GET;
	else if (type == "POST")
		this->type = POST;
	else if (type == "DELETE")
		this->type = DELETE;
	else {
		this->type = UNKNOWN;
		this->analysedReq.type = this->type;
		std::cout << "PARSING ERROR : Request type not found" << std::endl;
		this->code = 405; // Method Not Allowed
		return false;
	}
	if (this->type)
		this->analysedReq.type = this->type;
	return true;
}

/* Fill Map Headers  */
bool	Request::fillMapHeaders(std::string req, std::string line) {
	std::stringstream request(req);
	std::getline(request, line);
	while (!request.eof()) {
		std::getline(request, line); // Recover the line
		if (isEmpty(line)) // Check if the line is empty, if it contains only spaces or "^M".
			break;
		size_t pos = line.find(":");
		if (pos != std::string::npos) { // Check if there is a ":" in the line
			
			// Check if there is a similar key in the map to avoid double headers
			std::map<std::string, std::string>::iterator posDouble = this->headers.find(line.substr(0, pos));
			if (posDouble != this->headers.end()) {
				std::cout << "ERROR PARSING : There is two same headers!" << std::endl;
				this->code = 400; // Bad Request 
				return false;
			}

			// Insert headers line by line
			this->headers.insert(std::pair<std::string, std::string>(line.substr(0, pos), line.substr(pos + 2)));
			
			std::string accept = line.substr(0, pos);
			// Stock the content of the header accept 
			if (accept == "Accept")
				this->acceptHeader = line.substr(pos + 2);
			
		}
		else {
			std::cout << "PARSING ERROR : find() in constructor() fill map headers part" << std::endl;
			this->code = 400; // Bad Request
			return false;
		}
	}

	return true;
}

/* Fill map body  */
bool	Request::fillMapBody(std::string req, std::string line)  {
	std::stringstream request(req);
	std::getline(request, line);
	while (!request.eof()) {
		std::getline(request, line); // Recover the line
		if (isEmpty(line)) // Check if the line is empty
			break;
	}
	if (this->type == POST) {

		// Check if the content-length exists in the headers of the request
		std::map<std::string, std::string>::iterator PoscontLen = this->headers.find("Content-Length");
		if (PoscontLen == this->headers.end()) {
			std::cout << "PARSING ERROR : a body's content-length Needed" << std::endl;
			this->code = 411; // Length Required
			return false;
		}
		
		// Fill in the body
		while (!request.eof()) {
			std::getline(request, line);
			this->analysedReq.body += line;
		}
		
		// Case: the content-length is not equal to the body size
		size_t contLen = stoi(this->headers.at("Content-Length"));
		if (contLen  != this->analysedReq.body.size()) {
			std::cout << "PARSING ERROR : Bad body's content-length" << std::endl;
			// Case: invalid Content-Length
			this->code = 411; // Length Required
			return false;
		}

	}
/*	else if (!request.eof()) { // Case: where there would be an empty line 
							   // between the headers
		std::cout << "PARSING ERROR : Headers needs \":\" " << std::endl;
		this->code = 400; // Bad Request
		return false;
	}
*/
	// PRINT BODY
	std::cout << "BODY :" << this->analysedReq.body << std::endl;
	
	return true;
}


//Use analysis in the manufacturer 
//and in another function return the structured query

void Request::analyse(std::string req) {

	this->analysedReq.type = this->type;
	
	std::stringstream tmp(req);
	std::string line;
	std::getline(tmp, line);
	std::string file;

	// Search for the first occurence of "/" in the first line
	size_t pos = 0;
	pos = line.find("/");
	if (pos == std::string::npos) { // Check if the "/" has been found 
		std::cout << "ERROR : / not Found in analyse()" << std::endl;
		this->code = 400; // Bad Request
		return;
	}

	// Fill file with its path or name
	for (unsigned int i = pos; line[i] != ' ' && line[i]; i++)
		file.push_back(line[i]);
	
	if (file.size() == 1) {	   // Case: of the 1st html because nothing after the "/"
		this->analysedReq.file = file;
	}
	else if (file.size() > 0) // Other html pages or link requests than the 1st one
		this->analysedReq.file = file;
	else {					  // Case: If there is nothing, no files
		std::cout << "ERROR : in request URI;" << std::endl;
		this->code = 400; // Bad Request
					// J'hesite avec 404 Not Found
		return ;
	}

	// Fill the map headers if there is a url after the first line
	size_t poss = req.find("url");
	std::string url;
	if (poss != std::string::npos) { // Check if the "url" header has been found
		for (unsigned int i = poss + 5; req[i] != '\n' && req[i]; i++) 
			url += req[i];
		this->analysedReq.essentialHeaders.insert(std::pair<std::string, std::string>("url", url));
	}
			
	// Fill the map headers if there is an Host
	size_t findHost = req.find("Host");
	std::string host;
	if (findHost != std::string::npos) { // Check if the "url" header has been found
		for (unsigned int i = findHost + 5; req[i] != '\n' && req[i]; i++) 
			host += req[i];
		this->analysedReq.essentialHeaders.insert(std::pair<std::string, std::string>("Host", host));
	}
}

