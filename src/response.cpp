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
			return contentType;
		}
		else { // Case: there is no extension, 
			   // so the content-type is equal to the first one in the header accept
			ignoreQ();	
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
//	printFile(file, ext);

	// Manage and search in the header accept the MIME type corresponding to the extension
	contentType = manageExt(ext);

	return contentType;
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
//		std::cout << this->acceptHeader << std::endl;
	}
	else { // Case : "ext" not found
		// Content-type = first MIME type contained in accept	
		for (size_t i = 0; this->acceptHeader[i] != ',' && this->acceptHeader[i] != ';' && i < this->acceptHeader.size(); i++)
			contentType += this->acceptHeader[i];
	}
	return contentType;
}




/* Create response */
std::string	Request::createResponse(std::string body, int code) {
	
	int content_length = body.size(); 						// Stock the size of the body
	std::string response;
	
	// Display in the response the status code: ex: "HTTP/1.1 200 OK\n" Content-length ...
	response += "HTTP/1.1 ";
	std::stringstream ss; 									// To convert the code to string
	if (code != -1)
		this->code = code;
	ss << this->code;
	response += ss.str() + " ";
	if (Request::statusMsgs.find(this->code) != Request::statusMsgs.end())
		response += Request::statusMsgs.at(this->code); // Add code message
	response += "\r\n";
	
	// Case: Error 426, add if the protocol is not the right one
	/*if (this->code == 426) {
		response += "Upgrade: HTTP/1.1\n";
		response += "Connection: Upgrade\n";
	}*/

	// Display the date
	response += getDateHeader() + "\r\n";

	// Display in the answer, the length of the body:
	// Empty body string if there is no body
	if (content_length > 0) { // If there is a body

		// Add Content-type:
		response += "Content-Type: ";
		// Check the file extension and compare it with the header accept content
	
		if (this->acceptHeader.size() == 0) // Case: Request received without Accept Header
			response += "text/html";
		else
			response += checkFile(this->analysedReq.file); 
		response += "\r\n";
	// The file does not necessarily contain an extension,
	// in this case content-type = first content of Accept
		response += "Content-Length: ";
		std::stringstream ss1;
		ss1 << content_length;
		response += ss1.str();
		
		// Once the headers are added to the response
		// We then add the body received
		response += "\r\n\r\n";
		response += body;
	}	

	// Once the response is complete, 
	// the server sends the response with the send() function

	return response;
}
