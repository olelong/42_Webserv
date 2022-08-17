#include "../include/Request.hpp"

/* Fill in the type of request  */
bool	Request::fillTypeReq(std::string line) {
	std::string type;
	for (unsigned int i = 0; line[i] != ' ' && line.size() > 0; i++) {
		type.push_back(line[i]);
	}
	
	// PRINT type steps
	std::cout << std::endl << "Part type of request: " << std::endl;
	std::cout << "tmp: " << type << std::endl;
	std::cout << "line: " << line << std::endl;

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
	else if (!request.eof()) { // Case: where there would be an empty line 
							   // between the headers
		std::cout << "PARSING ERROR : Headers needs \":\" " << std::endl;
		this->code = 400; // Bad Request
		return false;
	}

	// PRINT BODY
	std::cout << "BODY :" << this->analysedReq.body << std::endl;
	
	return true;
}
