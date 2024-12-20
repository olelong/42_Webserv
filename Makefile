NAME    	= webserv

SRCS_D		= src/
SRCS   	 	= main.cpp Server.cpp ServerParser.cpp \
			  utilsReq.cpp Request.cpp response.cpp

OBJS_D    	= objs/
OBJS    	= ${SRCS:%.cpp=${OBJS_D}%.o}

CXX        	= c++
#CXX        = c++ -g3 -fsanitize=address
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -pedantic -pthread
RM        	= rm -f

${OBJS_D}%.o: $(SRCS_D)%.cpp
		mkdir -p ${OBJS_D}
		mkdir -p ${OBJS_D}request
		${CXX} ${CXXFLAGS} -c $< -o $@

all:    ${NAME}

${NAME}:    ${OBJS}
			${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS}

clean:
		rm -rf ${OBJS_D}

fclean:    clean
		${RM} ${NAME}

re:        fclean all

.PHONY:    all clean fclean re
