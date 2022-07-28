#include "../include/Request.hpp"

#define CYAN 		"\033[0;36m"
#define MAGENTA 	"\033[0;35m"
#define YELLOW 		"\033[0;33m"
#define END_COLOR 	"\033[0m"

int main(int argc, char *argv[])
{
	(void)argv;
	if (argc > 2)
	{
		std::cout << "Error : Too much arguments" << std::endl;
		return 1;
	}
	
	{
	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "test : fill type of the request" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "*******************************" << END_COLOR << std::endl << std::endl; 
	Request req("GET /https");
	std::cout << "code : " << req.getCode() << std::endl;
	std::cout << std::endl;
	Request req1("POST /https");
	std::cout << std::endl;
	Request req2("DELETE /https");
	std::cout << std::endl;
	Request req3("GETT /https");
	std::cout << std::endl;
	Request req4("GE T /https");
	std::cout << "code : " << req4.getCode() << std::endl;
	std::cout << std::endl;
	Request req5("");
	std::cout << "code : " << req5.getCode() << std::endl;
	std::cout << std::endl;
	std::string str("GET /HTTPS\nHOST: 127.0.1");
	Request reqq(str);
	std::cout << "code : " << reqq.getCode() << std::endl;
	std::cout << std::endl;
	}

	{
	std::cout << MAGENTA << "**********************************" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "test : fill headers of the request" << END_COLOR << std::endl; 
	std::cout << MAGENTA << "**********************************" << END_COLOR << std::endl << std::endl; 
	
	std::string str("GET /favicon.ico HTTP/1.1\n\
	Host: 127.0.0.1:8080\n\
	Connection: keep-alive\n\
	sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"102\", \"Google Chrome\";v=\"102\"\n\
	sec-ch-ua-mobile: ?0\n\
	User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36\n\
	sec-ch-ua-platform: \"Linux\"\n\
	Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\n\
	Sec-Fetch-Site: same-origin\n\
	Sec-Fetch-Mode: no-cors\n\
	Sec-Fetch-Dest: image\n\
	Referer: http://127.0.0.1:8080/\n\
	Accept-Encoding: gzip, deflate, br\n\
	Accept-Language: en-US,en;q=0.9"
	);

	Request req6(str);
	std::cout << std::endl;	
	std::cout << "code : " << req6.getCode() << std::endl;
	std::cout << std::endl;
	}

	return (0);
}
