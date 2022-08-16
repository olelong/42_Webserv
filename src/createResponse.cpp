#include "../include/Request.hpp"

/* Create response */
std::string	Request::createResponse(std::string body) {
	
	int content_length = body.size(); 						// Stock the size of the body
	std::string response;
	
	// Display in the response the status code: ex: "HTTP/1.1 200 OK\n" Content-length ...
	response += "HTTP/1.1 ";
	std::stringstream ss; 									// To convert the code to string
	ss << this->code;
	response += ss.str() + " ";
	response += Request::statusMsgs.at(this->code) + '\n'; // Add code message
	
	// Case: Error 426, add if the protocol is not the right one
	if (this->code == 426) {
		response += "Upgrade: HTTP/1.1\n";
		response += "Connection: Upgrade\n";
	}

	// Display in the answer, the length of the body:
	// Empty body string if there is no body
	if (content_length > 0) { // If there is a body

		response += "Content-Length : ";
		std::stringstream ss1;
		ss1 << content_length;
		response += ss1.str();
		response += '\n';
	
	// Add Content-type:
	response += "Content-Type: ";
	// Check the file extension and compare it with the header accept content
	response += checkFile(this->analysedReq.file); 
	response += '\n';
	// The file does not necessarily contain an extension,
	// in this case content-type = first content of Accept


	// Once the headers are added to the response
	// We then add the body received
		response += "\r\n\r\n";
		response += body;
	}

	// Once the response is complete, 
	// the server sends the response with the send() function

	// DEBUG : Print the response send to the server
	std::cout << std::endl << "Response send... : " << std::endl;
	std::cout << response << std::endl;
	std::cout << std::endl;


	return response;
}
