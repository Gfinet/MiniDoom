CC = GCC

CFLAGS = -Wall -Wextra -Werror

SRCS = 	ft_isascii/ft_isalpha.c \
	ft_isascii/ft_isdigit.c \
	ft_isascii/ft_isalnum.c \
	ft_isascii/ft_isascii.c \
	ft_isascii/ft_isprint.c \
	ft_isascii/ft_toupper.c \
	ft_isascii/ft_tolower.c \
	ft_mem/ft_memset.c \
	ft_mem/ft_bzero.c \
	ft_mem/ft_memcpy.c \
	ft_mem/ft_memmove.c \
	ft_mem/ft_calloc.c \
	ft_mem/ft_memchr.c \
	ft_mem/ft_memcmp.c \
	ft_str/ft_strlen.c \
	ft_str/ft_strlcpy.c \
	ft_str/ft_strlcat.c \
	ft_str/ft_strchr.c \
	ft_str/ft_strrchr.c \
	ft_str/ft_strncmp.c \
	ft_str/ft_strnstr.c \
	ft_str/ft_strdup.c \
	ft_str/ft_atoi.c \
	ft_str/ft_substr.c \
	ft_str/ft_strjoin.c \
	ft_str/ft_strtrim.c \
	ft_str/ft_split.c \
	ft_str/ft_itoa.c \
	ft_str/ft_strmapi.c \
	ft_str/ft_striteri.c \
	ft_put/ft_putchar_fd.c \
	ft_put/ft_putstr_fd.c \
	ft_put/ft_putendl_fd.c \
	ft_put/ft_putnbr_fd.c \
	GNL/get_next_line_bonus.c \
	GNL/get_next_line_utils_bonus.c

OBJ = $(SRCS:.c=.o)

BONUSSRC = ft_lst/ft_lstnew.c \
	ft_lst/ft_lstadd_front.c \
	ft_lst/ft_lstsize.c \
	ft_lst/ft_lstlast.c \
	ft_lst/ft_lstadd_back.c \
	ft_lst/ft_lstdelone.c \
	ft_lst/ft_lstclear.c \
	ft_lst/ft_lstiter.c \
	ft_lst/ft_lstmap.c
	

BONUSOBJ = $(BONUSSRC:.c=.o)

NAME = libft.a

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)
	@echo "library libft made"

%.o :	%.c
	@$(CC) $(CFLAGS) $< -c -o $@
	

bonus: $(BONUSOBJ) $(OBJ)
	@ar rcs $(NAME) $(BONUSOBJ) $(OBJ)

clean:
	@rm -f */*.o

fclean: clean
	@rm -f $(NAME)

re: fclean all

