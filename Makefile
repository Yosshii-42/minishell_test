NAME 	= minishell
SRCS	= src/main/main.c \
		  src/main/make_env.c \
		  src/main/utils.c \
		  src/main/free.c \
		  src/main/gloval_like_function.c \
		  src/main/safe_functions.c \
		  src/command/command.c \
		  src/command/open_file.c \
		  src/command/utils1.c \
		  src/command/utils2.c \
		  src/process/process.c \
		  src/process/process_ends.c \
		  src/process/utils.c \
		  src/process/specific_process.c \
		  src/signal/signal.c \
		  src/signal/signal_handler.c \
		  src/builtin/main_pwd.c \
		  src/builtin/echo.c \
		  src/builtin/cd.c \
		  src/builtin/cd_utils.c \
		  src/builtin/export.c \
		  src/builtin/export_utils.c \
		  src/builtin/exit.c \
		  src/builtin/exit_utils.c \
		  src/builtin/unset_env.c \
		  src/tokenizer/lexer.c \
		  src/tokenizer/lexer_add_kind.c \
		  src/tokenizer/tokenizer.c \
		  src/tokenizer/utils1.c \
		  src/tokenizer/utils2.c \
		  src/tokenizer/error.c \
		  src/tokenizer/expand_dollar.c \
		  src/tokenizer/expand_quote.c
OBJS	= $(SRCS:.c=.o)
CC		= cc
FLAGS	= -Wall -Wextra -Werror
LIBS	= -lreadline
# FLAGS   += -fsanitize=address -g
HEADDIR	= /src/
LIBFT	= ./libft/libft.a
# INCLUDES = -I$(RLDIR)/include -I$(HEADDIR)
# LIBDIRS  = -L$(RLDIR)/lib
# RLDIR   = $(shell brew --prefix readline)

#################################################################

%.o:%.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	make -C ./libft
	$(CC) $(FLAGS) $(LIBDIRS) -I$(HEADDIR) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME) 
	

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

# OS := $(shell uname -s)

# ifeq ($(OS), Linus)
# 	# commands for Linux
# endif

