NAME 	= minishell
SRCS	= src/main.c \
		  src/make_env.c \
		  src/command.c \
		  src/command_open_file.c \
		  src/command_utils1.c \
		  src/command_utils2.c \
		  src/process.c \
		  src/process_ends.c \
		  src/process_utils.c \
		  src/process_specific_process.c \
		  src/utils.c \
		  src/utils_free.c \
		  src/utils_gloval_like_function.c \
		  src/utils_safe_functions.c \
		  src/signal.c \
		  src/signal_handler.c \
		  src/builtin_main_echo_pwd.c \
		  src/builtin_cd.c \
		  src/builtin_export.c \
		  src/builtin_exit.c \
		  src/builtin_unset_env.c \
		  src/tokenizer_lexer.c \
		  src/tokenizer_lexer_add_kind.c \
		  src/tokenizer.c \
		  src/tokenizer_utils1.c \
		  src/tokenizer_utils2.c \
		  src/tokenizer_error.c \
		  src/tokenizer_expand_dollar.c \
		  src/tokenizer_expand_quote.c
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

