NAME 	= minishell
SRCS	= src/main.c \
		  src/set_env.c \
		  src/make_token_lst.c \
		  src/make_token_kind.c \
		  src/command.c \
		  src/command_utils.c \
		  src/open_file.c \
		  src/process.c \
		  src/process_utils.c \
		  src/utils.c \
		  src/free.c \
		  src/signal.c \
		  src/signal_handler.c
OBJS	= $(SRCS:.c=.o)
CC		= cc
FLAGS	= -Wall -Wextra -Werror
LIBS	= -lreadline
FLAGS   += -fsanitize=address -g
HEADDIR	= ./minishell.h
LIBFT	= ./libft/libft.a
INCLUDES = -I$(RLDIR)/include -I$(HEADDIR)
LIBDIRS  = -L$(RLDIR)/lib
RLDIR   = $(shell brew --prefix readline)

#################################################################

%.o:%.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	make -C ./libft
	$(CC) $(FLAGS) $(LIBDIRS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME) 
	

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

