# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile_Windows.mk                                :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/02 17:08:53 by shaas             #+#    #+#              #
#    Updated: 2022/08/02 17:08:53 by shaas            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=		cub3d

DIR_SRC		=		./src/
DIR_OBJ		=		./obj/
DIR_INC		=		./inc/
DIR_LIB		=		./lib/

CC			=		gcc
CC_FLAGS	=		-Wall -Wextra -Werror -g
MLX_FLAGS	=		$(DIR_LIB)MLX42/libmlx42.a -lglfw3 -lopengl32 -lgdi32

LIBFT_FLAGS =		$(LIBFT)

SRC			=		$(DIR_SRC)main.c \
					$(DIR_SRC)rgba.c \
					$(DIR_SRC)parser/parser.c \
					$(DIR_SRC)parser/get_scene_file_content.c \
					$(DIR_SRC)parser/get_scene_description_from_content.c\
					$(DIR_SRC)parser/has_valid_map.c
OBJ			=		$(SRC:$(DIR_SRC)%.c=$(DIR_OBJ)%.o)
LIBFT		=		$(DIR_LIB)libft/libft.a
MLX			=		$(DIR_LIB)MLX42/libmlx.a

all: $(MLX) $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CC_FLAGS) $(OBJ) -o $(NAME) $(MLX_FLAGS) $(LIBFT_FLAGS)
	@echo \e[91mc\e[95mu\e[94mb\e[96m3\e[92md \e[93mc\e[91mr\e[95me\e[94ma\e[96mt\e[92me\e[93md\e[93m!

$(DIR_OBJ)%.o: $(DIR_SRC)%.c
	@if not exist .\obj\\ md .\obj\\
	@$(CC) $(CC_FLAGS) -o $@ -c $<
	@echo $(notdir $<) compiled

$(MLX):
	@$(MAKE) --silent --directory $(dir $(MLX))

$(LIBFT):
	@$(MAKE) --silent --directory $(dir $(LIBFT))

run: all
	./$(NAME)

norm:
	@echo Checking Norminette on $(dir $(LIBFT))
	@echo norminette $(dir $(LIBFT))
	@norminette $(dir $(LIBFT)) | grep Error | egrep --color '.*Error!|$$' || true
	@echo Checking Norminette on $(DIR_SRC)
	@echo norminette $(DIR_SRC)
	@norminette $(DIR_SRC) | grep Error | egrep --color '.*Error!|$$' || true

clean:
	@if exist .\obj\\ del /S /Q .\obj\\
	@$(MAKE) --directory $(dir $(MLX)) clean
	@$(MAKE) --directory $(dir $(LIBFT)) clean
	@echo $(NAME) object files removed

fclean: clean
	@if exist $(NAME) del $(NAME)
	@$(MAKE) --directory $(dir $(MLX)) fclean
	@$(MAKE) --directory $(dir $(LIBFT)) fclean
	@echo $(NAME) removed

re: fclean all

.PHONY: all clean fclean re run norm

# text modifiers #
RED =				\e[31m
GREEN =				\e[32m
YELLOW =			\e[33m
BLUE =				\e[34m
MAGENTA =			\e[35m
CYAN =				\e[36m
LIGHTGRAY =			\e[37m
DARKGRAY =			\e[90m
LIGHTRED =			\e[91m
LIGHTGREEN =		\e[92m
LIGHTYELLOW =		\e[93m
LIGHTBLUE =			\e[94m
LIGHTMAGENTA =		\e[95m
LIGHTCYAN =			\e[96m
RED_BG =			\e[41m
GREEN_BG =			\e[42m
YELLOW_BG =			\e[43m
BLUE_BG =			\e[44m
MAGENTA_BG =		\e[45m
CYAN_BG =			\e[46m
LIGHTGRAY_BG =		\e[47m
DARKGRAY_BG =		\e[100m
LIGHTRED_BG =		\e[101m
LIGHTGREEN_BG =		\e[102m
LIGHTYELLOW_BG =	\e[103m
LIGHTBLUE_BG =		\e[104m
LIGHTMAGENTA_BG =	\e[105m
LIGHTCYAN_BG =		\e[106m
BOLD =				\e[1m
ITALIC =			\e[3m
UNDERLINED =		\e[4m
RESET =				\e[0m
