##
##  Makefile
##  TP2-Codec
##
##  Created by Clain Dimitri & Vandenbossche Louis on 2022-02-15.
##

SRC	=	TP2-Codec/main.cpp

OBJ	=	$(SRC:.cpp=.o)

RM	=	rm -f

CPP	=	g++ -o

CPPFLAGS	=	
LDFLAGS		=

NAME	=	Codec

all: $(NAME)

$(NAME): CPPFLAGS += -Wall -Wextra -std=c++11
$(NAME):$(OBJ)
	$(CPP) $(NAME) $(OBJ) $(CPPFLAGS) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)
re:	fclean all

.PHONY:	all clean fclean re