#include "../include/Request.hpp"

/* Checks the file extensionand the corresponding content-type */
std::string	Request::checkFile(std::string file) {
	
	std::string contentType;
	std::string ext;
	
	// Start from the end and find the 1st occurrence of "."
	size_t point = file.find_last_of(".");
	if (point == std::string::npos) { 			// Case: "." not found
		if (file == "/" && file.size() == 1) { 	// Case: 1st html page requested by the client
			contentType += "text/html";
			std::cout << "client ask for the 1rst html page" << std::endl;
			return contentType;
		}
		else { // Case: there is no extension, 
			   // so the content-type is equal to the first one in the header accept
			size_t commas = this->acceptHeader.find(',');
			size_t pointCom = this->acceptHeader.find(';');
			size_t nearestCom = pointCom;
			if (commas < pointCom)
				nearestCom = commas;
			contentType = this->acceptHeader.substr(0, nearestCom); 
			return contentType;
		}
	}
	
	// Fill in the content of the extension
	for (unsigned int i = point + 1; file[i]; i++)
		ext += file[i];
	

	// PRINT
	printFile(file, ext);


	// Manage and search in the header accept the MIME type corresponding to the extension
	contentType = manageExt(ext);

	return contentType;
}

void	Request::printFile(std::string file, std::string ext) {
	std::cout << "file: " << file << std::endl;
	std::cout << "extension: " << ext << std::endl;
}

/* Function to ignore the "q=*" in the content of accept */
void	Request::ignoreQ(void) {
	std::cout << "Accept Header with q=: " << this->acceptHeader << std::endl;
	for (std::string::iterator i = this->acceptHeader.begin();
		i != this->acceptHeader.end(); i++) {
		size_t q = this->acceptHeader.find("q=");
		if (q != std::string::npos) {
			size_t nb = 0;
			for (unsigned int j = q; this->acceptHeader[j] != ';' && this->acceptHeader[j] != ',' && j < this->acceptHeader.size(); j++)
				nb++;
			this->acceptHeader.erase(q, nb + 1);
		}
	}
	std::cout << "Accept Header without q=: " << this->acceptHeader << std::endl;
}

/* Manage extension */
std::string Request::manageExt(std::string ext) {
	std::string contentType;
	// Search if the extension is in the content of Accept
	size_t search = this->acceptHeader.find(ext);
	if (search != std::string::npos) { // Case : Find the extension in the header accept
		size_t count = 0;
		
		// We look for the "," or the ";" preceding the MIME type found 
		// to have the position of the beginning of this type
		for (size_t i = search; this->acceptHeader[i] != ',' && this->acceptHeader[i] != ';' && i > 0; i--)
			count++;
		
		// Content-type = MIME type corresponding to the extension
		int offset = search - count == 0 ? 0 : 1; //We find the type but it is the first of the header accept, otherwise we are on a comma "," so we must increment 
		for (size_t i = search - count + offset; this->acceptHeader[i] != ',' && this->acceptHeader[i] != ';' && i < this->acceptHeader.size(); i++)
			contentType += this->acceptHeader[i];
		std::cout << this->acceptHeader << std::endl;
	}
	else { // Case : "ext" not found
		// Content-type = first MIME type contained in accept	
		for (size_t i = 0; this->acceptHeader[i] != ',' && this->acceptHeader[i] != ';' && i < this->acceptHeader.size(); i++)
			contentType += this->acceptHeader[i];
	}
	return contentType;
}
