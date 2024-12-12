NAME 	= libft.a
SRCS	= ft_isdigit.c \
		  ft_strlen.c \
		  ft_split.c \
		  ft_memcmp.c \
		  ft_memset.c \
		  ft_strncmp.c \
		  ft_strnstr.c \
		  ft_strchr.c \
		  ft_strlcpy.c \
		  ft_strdup.c \
		  ft_calloc.c \
		  ft_bzero.c \
		  ft_putchar_fd.c \
		  ft_putstr_fd.c \
		  ft_putchar_fd.c \
		  ft_putstr_fd.c \
		  ft_putnbr_fd.c \
		  ft_itoa.c \
 		  ft_isdigit.c \
 		  ft_substr.c \
 		  ft_strjoin.c \
 		  ft_calloc.c

SRCS_PRINTF = ./printf/ft_printf.c \
			  ./printf/ft_put_and_count.c
OBJS	= $(SRCS:.c=.o)
OBJS_PRINTF = $(SRCS_PRINTF:.c=.o)
CC		= cc
FLAGS	= -Wall -Wextra -Werror 
HEADDIR	= ./includes

%.o:%.c
	$(CC) $(FLAGS) -I$(HEADDIR) -c $< -o $@

$(NAME): $(OBJS) $(OBJS_PRINTF)
	ar rcs $(NAME) $(OBJS) $(OBJS_PRINTF)

all: $(NAME)

clean:
	rm -f $(OBJS) $(OBJS_PRINTF)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
