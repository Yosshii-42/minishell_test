NAME 	= minishell
SRCS	= src/main.c \
		  src/set_env.c \
		  src/command.c \
		  src/command_utils.c \
		  src/process.c \
		  src/process_utils.c \
		  src/utils.c \
		  src/free.c
OBJS	= $(SRCS:.c=.o)
CC		= cc
FLAGS	= -Wall -Wextra -Werror
LIBS	= -lreadline
# FLAGS   += -fsanitize=address -g
HEADDIR	= ./minishell.h
LIBFT	= ./libft/libft.a

#################################################################

# %.o:%.c
# 	$(CC) $(FLAGS) -I$(HEADDIR) $-c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	make -C ./libft
	# $(CC) $(FLAGS) -I. $(HEADDER) $(LIBS) -o $(NAME) $(OBJS) 
	$(CC) $(FLAGS) -I. $(HEADDER) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

clean:
	make fclean -C ./libft
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: all
	./test.sh

.PHONY: all clean fclean re

#################################################################

OS := $(shell uname -s)

ifeq ($(OS), Linus)
	# commands for Linux
endif

