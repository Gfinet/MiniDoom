CC = GCC

CFLAGS = -Wall -Wextra -Werror

SRCS = ft_printf.c \
	ft_make_list.c \
	ft_ltoa_base.c \
	ft_strndup.c \
	ft_plst.c \
	ft_modif.c

LIB = $(LIBFT)libft.a

LIBFT = ./libft/

OBJ = $(SRCS:.c=.o)

NAME = libftprintf.a

.PHONY: all clean fclean re

all: $(NAME)
	@echo "library libftprintf made"

$(NAME): $(OBJ) libft.a
	@ar rcs $(NAME) $(OBJ)

%.o : %.c
	@$(CC) $(CFLAGS) $< -c -o $@ 


libft.a: 
	@make -C $(LIBFT) bonus
	@cp $(LIB) .
	@mv $(LIB) $(NAME)

clean:
	@rm -f *.o
	@make -C $(LIBFT) clean

fclean: clean
	@rm -f *.a
	@make -C $(LIBFT) fclean
	

re: fclean all


