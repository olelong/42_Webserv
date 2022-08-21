#include "../include/Request.hpp"
#include <fstream>

#define CYAN 		"\033[0;36m"
#define MAGENTA 	"\033[0;35m"
#define YELLOW 		"\033[0;33m"
#define END_COLOR 	"\033[0m"


void	printTypeReq(ReqType type) {
	
	if (type == 0)
		std::cout << "Type of the request: GET" << std::endl;
	else if (type == 1)
		std::cout << "Type of the request: POST" << std::endl;
	else if (type == 2)
		std::cout << "Type of the request: DELETE" << std::endl;
	else if (type == 3)
		std::cout << "Type of the request: UNKNOWN" << std::endl;
	else
		std::cout << "Type of the request: ERROR" << std::endl;
		
}

int main(void)
{

	{

	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "test : fill type of the request" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl << std::endl; 
	
	// Print type in the constructor
	Request req("GET /https");
	std::cout << "code : " << req.getCode() << std::endl;
	std::cout << std::endl;
	Request req1("POST /https HTTP/1.1");
	std::cout << std::endl;
	Request req2("DELETE /http s");
	std::cout << std::endl;
	Request req22("DELETE /http HTP/1.1");
	std::cout << std::endl;
	Request req3("GETT /https");
	std::cout << std::endl;
	Request req4("GE T /https");
	std::cout << "code : " << req4.getCode() << std::endl;
	std::cout << std::endl;
	Request req5("");
	std::cout << "code : " << req5.getCode() << std::endl;
	std::cout << std::endl;
	Request reqq("GET /HTTPS\nHOST: 127.0.1");
	std::cout << "code : " << reqq.getCode() << std::endl;
	std::cout << std::endl;
	


/************************************************************************************************/

		
	
	std::cout << MAGENTA << "**********************************" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "test : fill headers of the request" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "**********************************" << END_COLOR << std::endl << std::endl; 
	

	std::cout << YELLOW << "Example request:" << END_COLOR << std::endl;
	std::ifstream ifs("requests/simple");
	if (!ifs)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	Request req6(buffer.str());
	std::cout << std::endl;	
	req6.printFillMapHeaders();
	std::cout << "code : " << req6.getCode() << std::endl;
	std::cout << std::endl;
	
	std::cout << YELLOW << "-----------------------------" << END_COLOR << std::endl;
	
	std::cout << YELLOW << "Example request without body:" << END_COLOR << std::endl;
	std::ifstream ifss("requests/withoutBody");
	if (!ifss)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream bufferr;
	bufferr << ifss.rdbuf();
	Request req8(bufferr.str());
	std::cout << std::endl;	
	req8.printFillMapHeaders();
	std::cout << "code : " << req8.getCode() << std::endl;
	std::cout << std::endl;
	
	std::cout << YELLOW << "-----------------------------------------------" << END_COLOR << std::endl;
	
	std::cout << YELLOW << "Example request Not found Ext in accept Header:" << END_COLOR << std::endl;
	std::ifstream iifs("requests/NotFoundExt");
	if (!iifs)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream buuffer;
	buuffer << iifs.rdbuf();
	Request req10(buuffer.str());
	std::cout << std::endl;	
	req10.printFillMapHeaders();
	std::cout << "code : " << req10.getCode() << std::endl;
	std::cout << std::endl;

	std::cout << YELLOW << "---------------------------" << END_COLOR << std::endl;
	
	std::cout << YELLOW << "Example request First HTML:" << END_COLOR << std::endl;
	std::ifstream iffs("requests/firstHTML");
	if (!iffs)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream buffeer;
	buffeer << iffs.rdbuf();
	Request req11(buffeer.str());
	std::cout << std::endl;	
	req11.printFillMapHeaders();
	std::cout << "code : " << req11.getCode() << std::endl;
	std::cout << std::endl;

	std::cout << YELLOW << "---------------------------" << END_COLOR << std::endl;
	
	std::cout << YELLOW << "Example request POST:" << END_COLOR << std::endl;
	
	std::cout << std::endl;
	std::ifstream ifst("requests/post");
	if (!ifst)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream bbuffer;
	bbuffer << ifst.rdbuf();
	Request reeq(bbuffer.str());
	std::cout << std::endl;	
	reeq.printFillMapHeaders();
	std::cout << "code : " << reeq.getCode() << std::endl;
	std::cout << std::endl;
	
	std::cout << YELLOW << "---------------------------" << END_COLOR << std::endl;
	
	std::cout << YELLOW << "Example request DELETE:" << END_COLOR << std::endl;
	
	std::cout << std::endl;
	std::ifstream is("requests/delete");
	if (!is)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream _buffer;
	_buffer << is.rdbuf();
	Request _req(_buffer.str());
	std::cout << std::endl;	
	_req.printFillMapHeaders();
	std::cout << "code : " << _req.getCode() << std::endl;
	std::cout << std::endl;

	std::cout << std::endl;


/************************************************************************************************/


	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "     test : Get analysedReq   " << END_COLOR << std::endl; 
	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl << std::endl; 
	
	AnalysedRequest req7 = req6.getAnalysedReq();
	printTypeReq(req7.type);

	std::cout << "File of the request: " << req7.file << std::endl;
	std::cout << "Map of essentialHeaders of the request: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = req7.essentialHeaders.begin(); it != req7.essentialHeaders.end(); it++)
		std::cout << "key = " << it->first << " value = " << it->second << std::endl;
	std::cout << std::endl << std::endl;

	std::cout << YELLOW << "-----------------------------------" << END_COLOR << std::endl;
	std::cout << std::endl;

	AnalysedRequest req9 = req11.getAnalysedReq();
	printTypeReq(req9.type);
	std::cout << "File of the request: " << req9.file << std::endl;
	std::cout << "Map of essentialHeaders of the request: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = req9.essentialHeaders.begin(); it != req9.essentialHeaders.end(); it++)
		std::cout << "key = " << it->first << " value = " << it->second << std::endl;
	std::cout << std::endl;


	std::cout << YELLOW << "-----------------------------------" << END_COLOR << std::endl;
	std::cout << std::endl;
	AnalysedRequest req12 = req10.getAnalysedReq();
	printTypeReq(req12.type);
	std::cout << "File of the request: " << req12.file << std::endl;
	std::cout << "Map of essentialHeaders of the request: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = req12.essentialHeaders.begin(); it != req12.essentialHeaders.end(); it++)
		std::cout << "key = " << it->first << " value = " << it->second << std::endl;
	std::cout << std::endl;

	std::cout << YELLOW << "-----------------------------------" << END_COLOR << std::endl;
	std::cout << std::endl;
	AnalysedRequest req13 = reeq.getAnalysedReq();
	printTypeReq(req13.type);
	std::cout << "File of the request: " << req13.file << std::endl;
	std::cout << "Map of essentialHeaders of the request: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = req13.essentialHeaders.begin(); it != req13.essentialHeaders.end(); it++)
		std::cout << "key = " << it->first << " value = " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << "Body of the request: " << req13.body << std::endl;
	std::cout << std::endl;

	std::cout << YELLOW << "-----------------------------------" << END_COLOR << std::endl;
	std::cout << std::endl;
	
	AnalysedRequest req14 = _req.getAnalysedReq();
	printTypeReq(req14.type);
	std::cout << "File of the request: " << req14.file << std::endl;
	std::cout << "Map of essentialHeaders of the request: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = req14.essentialHeaders.begin(); it != req14.essentialHeaders.end(); it++)
		std::cout << "key = " << it->first << " value = " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << "Body of the request: " << req14.body << std::endl;
	std::cout << std::endl;

/************************************************************************************************/



	std::cout << std::endl;
	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "   test : Create a response    " << END_COLOR << std::endl; 
	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl << std::endl; 

	req6.createResponse("body nannnfddiogjl "); // 200 OK reponse avec body
	
	std::cout << YELLOW << "-----------------------------------" << END_COLOR << std::endl;
	std::cout << std::endl;
	req8.createResponse(""); // 200 OK
	std::cout << std::endl;
	
	std::cout << YELLOW << "-----------------------------------" << END_COLOR << std::endl;
	std::cout << std::endl;
	req.createResponse("");  // 400 car pas de header accept dans la requete
	std::cout << std::endl;
	
	std::cout << YELLOW << "-----------------------------------" << END_COLOR << std::endl;
	std::cout << std::endl;
	req10.createResponse(""); // 200 OK 200 OK 
	std::cout << std::endl;

	}

/************************************************************************************************/

	{

	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "     test : Parsing error   " << END_COLOR << std::endl; 
	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl << std::endl; 

	
	std::cout << YELLOW << "Header without \":\"" << END_COLOR << std::endl;
	
	std::ifstream ifs("requests/parsingError");
	if (!ifs)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	Request req(buffer.str());
	std::cout << std::endl;	
	req.printFillMapHeaders();
	std::cout << "code : " << req.getCode() << std::endl;
	std::cout << std::endl;

	
	std::cout << YELLOW << "-----------------------------------" << END_COLOR << std::endl;
	
	std::cout << YELLOW << "Double same Headers in the request:" << END_COLOR << std::endl;
	
	std::ifstream ifss("requests/doubleHeaders");
	if (!ifss)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream buffeer;
	buffeer << ifss.rdbuf();
	Request req1(buffeer.str());
	std::cout << std::endl;	
	req1.printFillMapHeaders();
	std::cout << "code : " << req1.getCode() << std::endl;
	std::cout << std::endl;
	

	std::cout << YELLOW << "--------------------------------------------" << END_COLOR << std::endl;
	
	std::cout << YELLOW << "Content in Accept Header with bad MIME type:" << END_COLOR << std::endl;
	
	
	std::ifstream iffs("requests/badTypeMIME");
	if (!iffs)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream bufffer;
	bufffer << iffs.rdbuf();
	Request req2(bufffer.str());
	std::cout << std::endl;	
	req2.printFillMapHeaders();
	std::cout << "code : " << req2.getCode() << std::endl;
	std::cout << std::endl;

	std::cout << YELLOW << "--------------------------------------------" << END_COLOR << std::endl;
	

	}

	{
	std::cout << YELLOW << "--------------------------------------------" << END_COLOR << std::endl;
	std::cout << YELLOW << "Differents tests:" << END_COLOR << std::endl;
	
	std::ifstream iffs("requests/tests");
	if (!iffs)
		std::cout << "Cannot open the file" << std::endl;
	std::stringstream bufffer;
	bufffer << iffs.rdbuf();
	Request req2(bufffer.str());
	std::cout << std::endl;	
	req2.printFillMapHeaders();
	//std::cout << "code : " << req2.getCode() << std::endl;
	req2.createResponse("oesfdks"); // 200 OK 200 OK 
	std::cout << std::endl;

	std::cout << YELLOW << "--------------------------------------------" << END_COLOR << std::endl;


	}

	return (0);
}
