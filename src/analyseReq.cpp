#include "../include/Request.hpp"

/* Analyse Request */
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
