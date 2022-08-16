#include "../include/Request.hpp"


/* Constructors & Destructors */

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

AnalysedRequest Request::getAnalysedReq(void) {
	return this->analysedReq;
}

