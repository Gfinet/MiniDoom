CC       = GCC
NAME = MiniDoom
CFLAGS   = -Wall -Wextra -Werror -Imlx -finline-functions -fvectorize -fslp-vectorize -ffast-math -falign-functions -funroll-loops -fstrict-aliasing -fomit-frame-pointer -flto -Ofast -O1 -O2 -Os -O3 -g -fsanitize=address
#FLAGS = -Wall -Wextra -Werror -Imlx -g3 -finline-functions -fvectorize -fslp-vectorize -ffast-math -falign-functions -funroll-loops -fstrict-aliasing -fomit-frame-pointer -flto -Ofast -O1 -O2 -Os -O3



SRCDIR = src
SRCS = 	src/MiniDoom.c				\
		src/handle_event.c			\
		src/handle_event2.c			\
		src/mini_maps.c 			\
		src/parse_maps.c 			\
		src/check_maps.c 			\
		src/check_arg.c 			\
		src/check_arg2.c 			\
		src/mlx_img.c 				\
		src/utils.c 				\
		src/life.c 					\
		src/movements.c 			\
		src/parse_weapon.c 			\
		src/interaction.c 			\
		src/draw_weapon.c 			\
		src/background.c 			\
		src/raycast.c 				\
		src/pause.c 				


OBJDIR = obj
OBJ      := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIB      = $(LIBFT)libftprintf.a
LIBFT    = ./inc/Printf/

MINI = ./inc/minilibx/
LIBX = $(MINI)libmlx.a

INC_BONUS = -I ./bonus/inc -I ./inc -I ./inc/minilibx

.PHONY: all clean fclean re bonus

all: $(NAME)
	

$(NAME): $(OBJ) libftprintf.a $(LIBX)
	@$(CC) $(CFLAGS) $^ -framework OpenGL -framework AppKit -o $(NAME)
	@echo "$(NAME) made"

bonus: $(BON_OBJ) libftprintf.a $(LIBX)
	@$(CC) $(CFLAGS) $(INC_BONUS) $^ -framework OpenGL -framework AppKit -o $(NAME)
	@echo "$(NAME) bonus made"

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -Imlx -c $< -o $@

add:
	git add bonus/ src/ inc/ maps/ texture/ Makefile .gitignore

$(LIBX):
	@make -C $(MINI)
	@echo "library mlx made"

libftprintf.a:
	@make -C $(LIBFT)
	@cp $(LIB) .
	@mv $(LIB) $(NAME)


clean:
	@rm -rf $(OBJ)
#@make -C $(MINI) clean
	@make -C $(LIBFT) clean
	@echo "clean done"

fclean: clean
	@rm -rf libftprintf.a
	@make -C $(LIBFT) fclean
	@rm -rf $(NAME)
	@echo "fclean done"
	
fclean_mlx:
	@make -C $(MINI) clean

re: fclean all
	@echo "make re done"

