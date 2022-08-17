#include "../include/Request.hpp"

/* Parsing error first line */
bool	Request::errorFirstLine(std::string line) {
		
		// Protocol HTTP/1.1
	std::string protocole;
	size_t p_begin = line.find_last_of(" ");
	if (p_begin == std::string::npos) {
		std::cout << "PARSING ERROR : Error in find() in the constructor()" << std::endl;
		this->code = 400; // Bad Request 
		return false;
	}
	for (unsigned int i = p_begin + 1; line[i] != ' ' && line[i]; i++) // Check the HTTP/1.1 protocol
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

/* Parsing error in the accept header of the request */
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

	// Count the number of slashes and commas:
	size_t nbCommas = 0; // Number of comma "," or semicolon ";"
	size_t nbSlashes = 0; // Number of  slashes
	for (unsigned int i = 0; this->acceptHeader[i]; i++) {	// Crosses the line
		if (this->acceptHeader[i] == ',' || this->acceptHeader[i] == ';')  // +1 nbCommas +1 if he finds one
			nbCommas += 1;
		if (this->acceptHeader[i] == '/') // +1 nbSlashes if he finds one
			nbSlashes += 1;
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

		if (isSlashInLastWord == false) { // Case: Error, did not find a "/" in the last word 
			this->code = 400; // Bad Request
			std::cout << "PARSING ERROR : Bad type MIME in Header Accept 1" << std::endl;
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
	if (nbSlashes == 1 && nbCommas == 0)
		return true;

	// Compare the number of slashes with the number of commas:
	if (nbSlashes != nbCommas) { // Case: Not the same number of "," or ";" and "/"
		this->code = 400; // Bad Request
		std::cout << "PARSING ERROR : Bad type MIME in Header Accept 2" << std::endl;
		return false;
	}
	
	return true;
}
