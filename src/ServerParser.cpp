#include "../include/ServerParser.hpp"

int ServerParser::lineNum = 0;

ServerParser::~ServerParser(void){}


// TAKE THE EACH 'WORDS' FROM THE LINE 
std::vector<std::string> getWords(const std::string& line)
{
    std::vector<std::string> words;	// Create vector to hold our words
    std::stringstream ss(line);
    std::string buf;				// A string to store the word on each iteration

    while (ss >> buf)
	{								// >> : extraction operator
        words.push_back(buf);
	}								// push 
	/*for (unsigned long i = 0; i < words.size(); i++)
	{
		std::cout << "the words: " << words[i] << std::endl;
	}*/
	std::string().swap(buf);
    return words;
}



bool  ServerParser::parse(std::string configFileName)
{
	std::string line;
	std::ifstream configFile(configFileName.c_str());

	if (!configFile)
	{
		std::cout << "Error: cannot open config file." << std::endl;
		return (false);
	}

	// on ignore les lignes jusqu'a ServerParser::lineNum
	// cela permet de diviser les blocs de server
	for (int i = 0; !configFile.eof() && i < ServerParser::lineNum; i++, std::getline(configFile, line))
	;

	bool isInsideLocation = false;  // to differentiate the server, location bloc

	int listen_cpt = 0; 			// can't have several port
	int serverName_cpt = 0; 		// can't have several server_name
	int root_cpt = 0;
	int real_cpt = 0;
//	int cgi_cpt = 0;				// can't have several cgi & cgi is required for location block
	int location_cpt = 0;

	while (!configFile.eof())
	{
		std::getline(configFile, line);
		std::vector<std::string> words = getWords(line);
		ServerParser::lineNum++;
	
		if (words.empty())
			continue;		

		if (words[0] != "#")
		{
			if (words[0] == "server")
			{
				// manage open bracket of server
				if (words.size() != 2)
				{
					std::cout << "Error! number of server error! >> " << line <<  std::endl;
					return (false);
				}
				this->servers = words[0].c_str();
				break;
			}
			else 
			{
				std::cout << "Config file error: " << words[0] << " is not a valid keyword." << std::endl;
				return false;
			}
		}
	}
	ServerParser::lineNum--;			// needed for the lineNum++;

	this->bodySize = 1000000; // set default max_body_size

	// boucle qui parcourt tout le fichier ligne par ligne
    while (!configFile.eof())
	{
		std::getline(configFile, line, '\n');
		
		// get the words from  the each line
		std::vector<std::string> words = getWords(line);

		ServerParser::lineNum++;
		if (words.empty())
			continue;

		if (words[0] == "#")
			continue;
	
		if (!isInsideLocation) 
        {
			// Gestion des erreurs de location
			if (words[0] == "location")				
           	{
				isInsideLocation = true;			// WAEL touch		
				real_cpt++;
				// on cree une nouvelle location (par defaut les
				// methodes http acceptees sont get, post et delete )
				Location newLocation;
				newLocation.acceptedMethods.push_back(HttpMethods::GET);
				newLocation.acceptedMethods.push_back(HttpMethods::POST);
				newLocation.acceptedMethods.push_back(HttpMethods::DELETE);

				newLocation.bodySize = 1000000; // default max_body_size
				this->locations.push_back(newLocation);
				this->locations.back().autoIndex = false;		// this solved error -> unitialized!
				// IF THERE ARE 3 words, we verify if the path is existed 
				if (words.size() == 3)
				{
					if (words[1].find_last_of('/') == words[1].size() - 1) // removing last / if theres any
						words[1].erase(words[1].size() - 1, 1);
                    this->locations.back().dir = words[1];
				
			/*		std::string pathName = words[1];	
					int fd;
					if ((fd = open(pathName.c_str(), O_RDONLY)) == -1)
					{
						std::cout << "Error! can't find the pathName  >> " << pathName << std::endl;
						return (false);
					}
					else
					{
						std::cout << "SUCCESS! we found the pathName >> " << pathName  << std::endl;	
						close(fd); 	// need to close de file descriptor 📙	
					}*/
				}
		
				// if the words size is not 3 	
				if (words.size() != 3)
				{
                  	std::cout << "Error on the location line " << std::endl;
                    return (false);	
				}
			} // end of words[0] == "location"

		} 	// end loop of if (!isInsideLocation)
		

/*
  _                 _   _             
 | | ___   ___ __ _| |_(_) ___  _ __  
 | |/ _ \ / __/ _` | __| |/ _ \| '_ \
 | | (_) | (_| (_| | |_| | (_) | | | |
 |_|\___/ \___\__,_|\__|_|\___/|_| |_|
                                      
*/

			if (isInsideLocation) 
			{	
               	isInsideLocation = true;

					// WHEN LOCATION BLOC IS EMPTY -> ERROR	
					if (real_cpt == 1)
					{
						if (words[0] != "index" && words[0] == "}")
						{
							std::cout << "Error! LOCATION BLOC IS EMPTY " << std::endl;
							return (false);
						}
					}
					
					// 	YOOYOO TEST - worked : to detect the "location" keyword inside the location block
					if (words[0] == "location")
					{
						location_cpt++;
						if (location_cpt == 2)
						{
							std::cout << "in location: location is invalid keyword" << std::endl;
							return false;
						}
					}

					// INDEX
					else if (words[0] == "index")
					{
						real_cpt++;
						if (words.size() != 2)
                    	{
                        	std::cout << "Error of number of index " << std::endl;
                        	return false;
                    	}

						if (words[1].find('/') != 0)
							words[1].insert(0, "/");
                    	this->locations.back().indexes.push_back(words[1].c_str());
							
						size_t indexPos = words[1].find(".");
						if (indexPos == std::string::npos)
						{
							std::cout << "Error! INDEX  file extension is incorrect >>  " << words[1] 	<< std::endl;
							return (false);
						}
						std::string cut = words[1].substr(indexPos);
						if (cut != ".html")
						{
							std::cout << "Error! INDEX  file extension is incorrect >>  " << cut 	<< std::endl;
							return (false);
						}
					}
		
					// LIMIT_EXCEPT : GET 
					else if (words[0] == "http_accept")
                	{
						real_cpt++;
                    	if (words.size() < 2 || words.size() > 4)
                    	{
                        	std::cout << "Error of number of http_accept " << std::endl;
                        	return false;
                    	}
						this->locations.back().acceptedMethods.clear();
                    	for (unsigned long i = 1; i < words.size(); i++)
						{
							if (words[i] == "GET")
								this->locations.back().acceptedMethods.push_back(HttpMethods::GET);
							else if (words[i] == "POST")
								this->locations.back().acceptedMethods.push_back(HttpMethods::POST);
							else if (words[i] == "DELETE")
								this->locations.back().acceptedMethods.push_back(HttpMethods::DELETE);
							else
							{
								std::cout << "Unknown keyword after http_accept" << std::endl;
								return false;
							}
						}
                	}

					// AUTOINDEX
					else if (words[0] == "autoindex")
					{
						this->locations.back().autoIndex = false; // par defaut, autoindex= "false"
						real_cpt++;
						if (words.size() != 2)
						{
							std::cout << "Error of number of autoindex " << std::endl;
							return false;
						}
						if (words[1] == "on")
							this->locations.back().autoIndex = true;
						else if (words[1] == "off")
							this->locations.back().autoIndex = false;
						else
						{
							std::cout << "autoindex should be 'on' or 'off' >> " << words[1] << std::endl;
							return false;
						}
					}
					


					// LOCATION	
					// ROOT 
					else if (words[0] == "root")
					{
						real_cpt++;
            			if (words.size() != 2)
            			{
                			std::cout << "Error of number of root! " << std::endl;
                			return (false);
            			}
						if (words.size() == 2) 
						{	
							if (words[1].find_last_of('/') == words[1].size() - 1) // removing last / if theres any
								words[1].erase(words[1].size() - 1, 1);
                    		this->locations.back().root = words[1];
				
							std::string rootPath = words[1];	
							int fd;
							if ((fd = open(rootPath.c_str(), O_RDONLY)) == -1)
							{
								std::cout << "Error! can't find the root path!  >> " << rootPath << std::endl;
								return (false);
							}
							else
							{
								//std::cout << "SUCCESS! we found the rootPath >> " << rootPath  << std::endl;	
								close(fd); 	// need to close de file descriptor 📙	
							}
						}
					}
		

					// LOCATION
					// client max bodysize"
					else if (words[0] == "client_max_body_size")
					{
						real_cpt++;
						if (words.size() != 2)
						{
							std::cout << "Error of number of client_max_body_size! " << std::endl;
							return (false);
						}
						this->locations.back().bodySize = std::strtoul(words[1].c_str(), NULL, 0);
					}
					
					// LOCATION
					// UPLOAD
					else if (words[0] == "upload")
					{
						real_cpt++;
						if (words.size() != 2)
						{
							std::cout << "Error of number of upload in server! " << std::endl;
							return (false);
						}
						this->locations.back().upload = words[1];
					}

					// CGI NEED TO BE INSIDE LOCATION!!!
					else if (words[0] == "cgi")
					{
						//cgi_cpt++;
						if (words.size() != 3)
						{
							std::cout << "Error of number of CGI " << std::endl;
							return (false);
						}
						
						//std::cout << "CGI_CPT ? " << cgi_cpt << std::endl;
						// if key is missing . 
						size_t pos = words[1].find(".");	
						if (pos == std::string::npos)
						{
							std::cout << "Error: cgi extension file must begin with a '.' >> " << words[1] << std::endl;
							return (false);
						}

						/*std::string cut = words[1].substr(pos);
						if (cut != ".php")
						{
							std::cout << "Error! cgi file extension is incorrect >>  " << cut 	<< std::endl;
							return (false);
						}*/

						// if key is a number -> error
						for (unsigned long i = 0; i < std::strlen(words[1].c_str());i++)
						{
							if (std::isdigit(words[1].c_str()[i]))
							{
								std::cout << "Error! file extensions cant have number! >> " << words[1] << std::endl;
								return (false);
							}
						}		

						// If cgi variables words[2] is number -> error
						/*for (unsigned long i = 0; i < std::strlen(words[2].c_str()); i++)
						{
							if (std::isdigit(words[2].c_str()[i]))
							{
								std::cout << "Error! can't have number in! >>" << words[2] << std::endl;
								return (false);
							}
						}*/
						this->locations.back().cgi[words[1].c_str()] = words[2];
					}
				
					// CHECK THE CLOSE BRACKET INSIDE THE LOOP
					else if (words[0] == "}")
					{
						real_cpt++;
					}
					else if ((words[0].size() == 1 && words[0] != "}") || (words[0].empty() && words[0] != "}" ))
					{
						std::cout << "Error! need to be closed } " << std::endl;
						return false;
					}

					// ✅ CHECK : WORDS[0] _INSIDE_LOCATION -> PARSING ERROR 
					else 
					{
					 	if  (isInsideLocation)		// need to add: isInsideLocation 
						{
							//if (words[0] != "cgi" || words[0] != "client_max_body_size" || words[0] != "index" || words[0] != "autoindex" || words[0] != "root" || words[0] != "limit_except")
							{	
								std::cout << "Error! LOCATION BLOCK element invalid 🍰  >>>  " << words[0]  << std::endl;	
								return (false);
							}
						}
					}
				}				// IF (isInsideLocation)- END


/*  
  ___  ___ _ ____   _____ _ __ 
 / __|/ _ \ '__\ \ / / _ \ '__|
 \__ \  __/ |   \ V /  __/ |   
 |___/\___|_|    \_/ \___|_|   
                               
 */

        // server_name
        if (words[0] == "server_name")
        {
			serverName_cpt++;
           if (words.size() != 2)
           {
              std::cout << "Error of number of  server_name number! " << std::endl;
              return (false);
           }
		   else if (serverName_cpt != 1)
		   {
			   std::cout << "Error! server_name should be one and only! "<< line << std::endl;
			   return (false);
		   }
		
		// If the server_name is a number ==> error
		 for (unsigned long i = 0; i < std::strlen(words[1].c_str()); i++)
		 {
			 if (std::isdigit(words[1].c_str()[i]))
			 {
				 std::cout << "Error! server_name can't be the number! >> " << words[1] << std::endl;
				 return (false);
			 }
		}
		 this->name = words[1].c_str();
		}

        // listen
		else if (words[0] == "listen")
        {
			listen_cpt++;

//			std::cout << "what is listen.size? " << words.size() << std::endl;
	/*		if (words.size() == 3)
			{
				if (!isInsideLocation)
				{
					if (words[2] != "default_server")
					{
						std::cout << "Error! should be as :  default_server >> " << words[2]  << std::endl;
						return (false);
					}
				}
				if (words[2] == "default_server")
				{
					if (words.size() != 3)
					{
						std::cout << "Error of number of listen! (default_server) " << std::endl;
						return (false);
					}
				}
			}*/
			if (words.size() != 2)
            {
                std::cout << "Error of number of listen! " << std::endl;
                return (false);
            }

			// find the same port number in the server block 
			/*int samePort = std::strtod(words[1].c_str(), NULL);
			if (std::find(this->ports.begin(), this->ports.end(), samePort) != this->ports.end())
			{
				std::cout << "ERROR! CAN'T HAVE THE SAME " << words[1] << " PORT 🥺 " << std::endl;
				return (false);
			}*/
			
			if (listen_cpt > 1)
			{
				std::cout << "Error! can't have several port " << std::endl;
				return (false);
			}

			// TO DETECT THE WORDS[1] ==  NOT A NUMBER VALUE!
			// PRINT THE INSIDE OF THE WORDS[1] INDIVIUALLY!
			for (unsigned long  i = 0; i < std::strlen(words[1].c_str()); i++)
			{
				if (std::isdigit(words[1].c_str()[i]))
					continue;
				else
				{
					std::cout << "Error! Port error, this is NOT A NUMBER >> "  << words[1] << std::endl;
					return (false);
				}
			}
			this->port = std::strtod(words[1].c_str(), NULL);
		}


	
		// [SERVER] root && !(isInsideLocation) !!!!!!!!!!!!!! 
		else if (words[0] == "root" && !(isInsideLocation))
		{
			root_cpt++;
			if (words.size() != 2)
			{
				std::cout << "Error of number of root in server! " << std::endl;
				return (false);
			}
			if (root_cpt > 1)
			{
				std::cout << "Error! can't have the several roots in server! " << std::endl;
				return (false);
			}
			if (words.size() == 2) 
			{
				std::string rootPath = words[1];	
				int fd;
				if ((fd = open(rootPath.c_str(), O_RDONLY)) == -1)
				{
					std::cout << "Error! can't find the root path!  >> " << rootPath << std::endl;
					return (false);
				}
				else
				{
					//std::cout << "SUCCESS! we found the rootPath >> " << rootPath  << std::endl;	
					close(fd); 	// need to close de file descriptor 📙	
				}
			}

		

			// TO DETECT THE WORDS[1] ==  NOT A NUMBER VALUE!
			for (unsigned long i = 0; i < std::strlen(words[1].c_str());i++)
			{
				if (std::isdigit(words[1].c_str()[i]))
				{
					std::cout << "Error! can't have number in root >> " << words[1] << std::endl;
					return (false);
				}
			}
			if (words[1].find_last_of('/') == words[1].size() - 1) // removing last / if theres any
				words[1].erase(words[1].size() - 1, 1);
			this->root = words[1].c_str();
		}

		// [SERVER] client_max_body_size  && !(isInsideLocation) !!!!!!!!!!!!!! 
		else if (words[0] == "client_max_body_size" && !(isInsideLocation))
		{
			if (words.size() != 2)
			{
				std::cout << "Error of number of client_max_body_size! " << std::endl;
				return (false);
			}

			for(unsigned long i = 0; i < std::strlen(words[1].c_str());i++)
			{
				if (std::isdigit(words[1].c_str()[i]))
					continue ;
				else 
				{
					std::cout << "Error! cleint_max_body_size : Only numbers are allowed! " << words[1] << std::endl;
					return (false);
				}
			}
			this->bodySize = std::strtoul(words[1].c_str(), NULL, 0);	
		}

	
		else if (words[0] == "error_page")
		{
			if (words.size() != 3)
			{
				std::cout << "Error of number of error_page! " << std::endl;
				return (false);
			}	
			int errorCode = std::strtod(words[1].c_str(), NULL);
			if (this->errorPages.find(errorCode) != this->errorPages.end()) // si la key existe deja
			{
				std::cout << "Error: redefining error page " << words[1] << std::endl;
				return (false);
			}
		  	this->errorPages[errorCode] = words[2];		// WHY ? this is  same as push_back in vector
			
			/* LOGIC 
			   1. VERIFY if the error code is correct
			   		- if the error code is not correct(not a number, number doesnt exist) => error
			   2. VERIFY if the path of the error code exists,
			   	- 3. if it's not existed => error
				- 4. if the error code is existed,
			   		-5. But if the "error code" (KEY) and VALUE is NOT matched => error
					-6. This is the valid code number, but if we didn't define => error
			*/

			std::string pagePath = words[2];
			int fd;
			if ((fd = open(pagePath.c_str(), O_RDONLY)) == -1)
			{
				std::cout <<  "Error! can't find the Error_Pages_Path 🥗 >> " << pagePath << std::endl;
				return false;
			}
			else
			{
				close(fd);
			//	std::cout << "SUCCESS! we found the Error_page_path ✨ : " << pagePath << std::endl;
			/*switch (errorCode)
			{
				// 400 -> CLIENT CODE ERROR
				case BAD_REQUEST:
				{
					if (words[2] != DIR_400)
					{
						std::cout << "Error! KEY and VALUE are not corresponded! >>> " << "value: " <<  words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case UNAUTHORIZED:
				{
					if (words[2] != DIR_401)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case FORBIDDEN:
				{
					if (words[2] != DIR_403)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case NOT_FOUND:
				{
					if (words[2] != DIR_404)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case NOT_ALLOWED:
				{
					if (words[2] != DIR_405)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case GONE:	
				{
					if (words[2] != DIR_410)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case PAYLOAD_LARGE:
				{
					if (words[2] != DIR_413)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;

				}

				// 500 -> SERVER CODE ERROR
				case INTERNAL_SERVER:
				{
					if (words[2] != DIR_500)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case BAD_GATEWAY:
				{
					if (words[2] != DIR_502)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case SERVICE_UNAVAILABLE:
				{
					if (words[2] != DIR_503)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case GATEWAY_TIMEOUT:
				{
					if (words[2] != DIR_504)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				case HTTP_NOTSUPPORT:
				{
					if (words[2] != DIR_505)
					{
						std::cout << "Error! KEY and VALUE are not corresponded!  >>> " << words[2] << std::endl;
						return (false);
					}
					break ;
				}
				default:
						std::cout << "Error! not defined error_pages of http >> " << words[1] << std::endl;
				//		break ;
						return (false);
				}*/ 	// end of switch {}
			} 		// end -> else {}  when we found a path that existed

			// 1) Need to verify if the words[2] finished in .html	

			size_t pos = words[2].find(".");	
			if (pos == std::string::npos)
			{
				std::cout << "Error: Error page file does not have any .html extension >> " << words[2] << std::endl;
				return (false);
			}
			std::string cut = words[2].substr(pos);
			if (cut != ".html")
			{
				std::cout << "Error! error_pages file extension is incorrect >>  " << cut 	<< std::endl;
				return (false);
			}
		}
	
		// END OF THE SERVER 
		// manage close bracket of server
		else if (words[0] == "}")
		{
			if (words.size() != 1)
			{
				std::cout << "Error: Close brackets must be alone in a line" << std::endl;
				return (false);
			}
			if (isInsideLocation)
			{
				isInsideLocation = false;
				location_cpt = 0;
				if (this->locations.back().root.empty()) {
					std::cout << "Error: there must be a root inside location block" << std::endl;
					return false;
				}
			}
			else
				break;
		}

		else
		{
			if  (std::find (words.begin(), words.end(), "server") == words.end() && !isInsideLocation)
			{
				// I don't need it ? ===> it works..
				//if (words[0] != "client_max_body_size" && words[0] != "listen" && words[0] != "server_name" && words[0] != "root" && words[0] != "error_page")
				{
					std::cout << "Find SERVER error 🐷 >>>>  " << words[0]  << std::endl;	
					return (false);
				}
				if (isInsideLocation)
				{
					isInsideLocation = false;
					location_cpt = 0;
				}
				else 
					break ;
			}
		}
	}
	
	if (serverName_cpt == 0)
	{
		std::cout << "Error! on the server block! "  << std::endl;
		return (false);
	}
	else if (listen_cpt == 0)
	{
		std::cout << "Error! LISTEN directive needed!  " << std::endl;
		return (false);
	}
	else if (root_cpt == 0)
	{
		std::cout << "Error! ROOT directive needed inside server block! " << std::endl;
		return (false);
	}
	ServerParser::lineNum++;

	if (this->portError() == -1)
		return (false);
	return (true);
}


/* Define the errors of the port
	1) Port Ranges :  Port size
*/	
int ServerParser::portError()
{
	if (this->port < 0 || this->port > 65535)
	{
		std::cout << PINK <<"Error: port should be in range 0-65535" << std::endl;	
		return (-1);
	}
    return (1);
}

// Debug : print for debug
void ServerParser::print()
{
	//  SERVER
	if (this->servers.empty())
		std::cout << BLUE << "SERVER IS EMPTY! " << std::endl;
	else
		std::cout << BLUE <<  "THE SERVER: " << this->servers << std::endl;

	std::cout << RED << "Server name "  <<  ": " << this->name << std::endl;
	std::cout << ORANGE << "Port: " << this->port << std::endl;

	if (this->root.empty())
		std::cout << YELLOW << "Root: is empty >_< " << std::endl;
	else	
		std::cout <<  YELLOW << "Root: " << this->root<< std::endl;	
	std::cout << PINK << "[SERVER] client_body_size limit : " << this->bodySize  << std::endl;

	for(std::map<int, std::string>::iterator it = this->errorPages.begin(); it != this->errorPages.end(); it++)	
		std::cout << GREEN << "Error_page : "  << "key : " << it->first << ", value : " << it->second << std::endl;


	// INSIDE THE REAL LOCAITON ================>
	for (unsigned long i = 0; i < this->locations.size(); i++)
	{
		std::cout << VIOLET << "------------" << std::endl;
		std::cout << VIOLET << "Location " << i + 1 << ": " << std::endl;
		std::cout << VIOLET << "Directory: " << this->locations[i].dir << std::endl;

		//MAP -> CGI 
		if (this->locations[i].cgi.empty())
			std::cout << WHITE << "[location] CGI is empty" << std::endl;
		for(std::map<std::string, std::string>::iterator it = this->locations[i].cgi.begin(); it != this->locations[i].cgi.end(); it++)
			std::cout << PEACH << "CGI : " << "key : " << it->first << ", value : " <<  it->second << std::endl;
		if (this->locations[i].root.empty())
			std::cout << YELLOW << "[location] Root is empty >_< " << std::endl;
		else if (!this->locations[i].root.empty())
			std::cout << YELLOW << "[location] Root: " << this->locations[i].root << std::endl;

		if (this->locations[i].upload.empty())
			std::cout << WHITE << "[location] Upload is empty" << std::endl;
		else
			std::cout << PINK << "Upload : " << this->locations[i].upload << std::endl;
		if (!(this->locations[i].bodySize == 0))
			std::cout << GREEN << "Client max body size : " << this->locations[i].bodySize << std::endl;	
	
		for (unsigned long j = 0; j < this->locations[i].indexes.size(); j++)
			std::cout << VIOLET <<  "Index " << j << ": " << this->locations[i].indexes[j] << std::endl;
		std::cout <<  BLUE << std::boolalpha << "Autoindex: " << this->locations[i].autoIndex << std::endl;	

		std::vector<HttpMethods::type> v = this->locations[i].acceptedMethods;
		{	
			std::cout << VIOLET << "Accepted methods: " << std::endl;
			// par defaut : si c'est pas ecrit (http_accept) ==> on accept tout
			std::cout << (std::find(v.begin(), v.end(), HttpMethods::GET) != v.end() ? "GET " : "") << std::endl;
			std::cout << (std::find(v.begin(), v.end(), HttpMethods::POST) != v.end() ? "POST, " : "") << std::endl;
			std::cout << (std::find(v.begin(), v.end(), HttpMethods::DELETE) != v.end() ? "DELETE" : "") << std::endl;
		}
	}
    std::cout << WHITE << "============================ " << std::endl;
}

// Methode statique qui balaye tout le fichier avant le vrai parsing
int ServerParser::nbOfServers(std::string configFileName)
{
	std::ifstream configFile(configFileName.c_str());

	if (!configFile)
	{
		std::cout << "Error: Config file name error 🪧" << std::endl;
		return (-1);
	}

	int server_cpt = 0;
	int cpt = 0;
	int open_bracket = 0, close_bracket = 0;
	std::string line;
	while (!configFile.eof())
	{
		std::getline(configFile, line);
		std::vector<std::string> words = getWords(line);
		if (words.empty())
			continue;
		if (words[0] == "#")
			continue;
		if (words[0] == "server")
			cpt++;
		if (words[0] == "server")
			server_cpt++;
		else
			server_cpt = 0;
		if (server_cpt > 1)
		{
			std::cout << "Error! inside server, we can't have several sever block!" << std::endl;
			return (-1);
		}

	// Verify the { } if brackets are closed or not
		for (unsigned long i = 0; i < words.size(); i++)
		{
			if (words[i] == "{")
				open_bracket++;
			else if (words[i] == "}")
				close_bracket++;
		}
	}
	if (open_bracket != close_bracket)
	{
		std::cout << "Bracket error in config file!" << std::endl;
		return -1;
	}
	return cpt;
}
