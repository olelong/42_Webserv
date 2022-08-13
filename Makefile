NAME    	= webserv

SRCS_D		= src/
#SRCS   	 	= main.cpp Server.cpp Request.cpp
SRCS   	 	= Request.cpp  mainReq.cpp

OBJS_D    	= objs/
OBJS    	= ${SRCS:%.cpp=${OBJS_D}%.o}

#CXX        	= c++
CXX        = c++ #-g3 -fsanitize=address
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -pedantic
RM        	= rm -f

${OBJS_D}%.o: $(SRCS_D)%.cpp
		mkdir -p ${OBJS_D}
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
