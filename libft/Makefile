NAME 	= libft.a
SRCS	= ft_strlen.c \
		  ft_split.c \
		  ft_memset.c \
		  ft_memcmp.c \
		  ft_strncmp.c \
		  ft_strnstr.c \
		  ft_strchr.c \
		  ft_strlcpy.c \
		  ft_strdup.c \
		  ft_calloc.c \
		  ft_bzero.c \
		  ft_putchar_fd.c \
		  ft_putstr_fd.c
# 		  ft_isalpha.c \
# 		  ft_isdigit.c \
# 		  ft_isalnum.c \
# 		  ft_isascii.c \
# 		  ft_isprint.c \
# 		  ft_strlen.c \
# 		  ft_memset.c \
# 		  ft_bzero.c \
# 		  ft_memcpy.c \
# 		  ft_memmove.c \
# 		  ft_strlcpy.c \
# 		  ft_strlcat.c \
# 		  ft_toupper.c \
# 		  ft_tolower.c \
# 		  ft_strchr.c \
# 		  ft_strdup.c \
# 		  ft_strrchr.c \
# 		  ft_strncmp.c \
# 		  ft_memchr.c \
# 		  ft_memcmp.c \
# 		  ft_strnstr.c \
# 		  ft_atoi.c \
# 		  ft_atol.c \
# 		  ft_calloc.c \
# 		  ft_substr.c \
# 		  ft_strjoin.c \
# 		  ft_strtrim.c \
# 		  ft_split.c \
# 		  ft_itoa.c \
# 		  ft_strmapi.c \
# 		  ft_striteri.c \
# 		  ft_putchar_fd.c \
# 		  ft_putstr_fd.c \
# 		  ft_putendl_fd.c \
# 		  ft_putnbr_fd.c \
# 		  get_next_line.c
# SRCS_B	= ft_lstnew.c \
# 		  ft_lstadd_back.c \
# 		  ft_lstlast.c \
# 		  ft_lstdelone.c \
# 		  ft_lstclear.c \
# 		  ft_lstiter.c \
# 		  ft_lstadd_front.c \
# 		  ft_lstsize.c 
SRCS_PRINTF = ./printf/ft_printf.c \
			  ./printf/ft_put_and_count.c
OBJS	= $(SRCS:.c=.o)
OBJS_B	= $(SRCS_B:.c=.o)
OBJS_PRINTF = $(SRCS_PRINTF:.c=.o)
CC		= gcc
FLAGS	= -Wall -Wextra -Werror 
HEADDIR	= ./includes

%.o:%.c
	$(CC) $(FLAGS) -I$(HEADDIR) -c $< -o $@

$(NAME): $(OBJS) $(OBJS_B) $(OBJS_PRINTF)
	ar rcs $(NAME) $(OBJS) $(OBJS_B) $(OBJS_PRINTF)

# bonus: $(NAME) $(OBJS_B) 
# 	ar rcs $(NAME) $(OBJS_B) 

all: $(NAME)

clean:
	rm -f $(OBJS) $(OBJS_B) $(OBJS_PRINTF)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
