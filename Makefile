# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/13 22:43:00 by shaas             #+#    #+#              #
#    Updated: 2022/07/05 22:12:07 by shaas            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=		cub3d

DIR_SRC		=		./src/
DIR_OBJ		=		./obj/
DIR_INC		=		./inc/
DIR_LIB		=		./lib/

CC			=		cc
CC_FLAGS	=		-Wall -Wextra -Werror -g
MLX_FLAGS	=		$(DIR_LIB)MLX42/libmlx42.a -lglfw -L $(shell brew --prefix glfw)/lib

#OS := $(shell uname)
#ifeq ($(OS), Darwin)
#	CPPFLAGS	=	-I$(HOME)/.brew/opt/readline/include
#	LDFLAGS		=	-L$(HOME)/.brew/opt/readline/lib -lreadline
#	LREADLINE	=
#endif
#ifeq ($(OS), Linux)
#	CPPFLAGS	=
#	LDFLAGS		=
#	LREADLINE	=	-lreadline
#endif

SRC			=		$(DIR_SRC)main.c \
					$(DIR_SRC)other_function.c \
					$(DIR_SRC)rgba.c
OBJ			=		$(SRC:$(DIR_SRC)%.c=$(DIR_OBJ)%.o)
LIBFT		=		$(DIR_INC)libft/libft.a
MLX			=		$(DIR_LIB)MLX42/libmlx.a

all:			$(MLX) $(NAME)

$(NAME):		$(OBJ)
				@$(CC) $(CC_FLAGS) $(OBJ) -o $(NAME) $(MLX_FLAGS)
				@printf $(CYAN)"$(NAME) created\n"$(RESET)

$(DIR_OBJ)%.o:	$(DIR_SRC)%.c
				@mkdir -p $(dir $@)
				@$(CC) $(CC_FLAGS) -c $< -o $@
				@printf "$(notdir $<) compiled\n"

$(MLX):
				@$(MAKE) --silent --directory $(dir $(MLX))

run:			all
				./$(NAME)

clean:
				@rm -rf $(DIR_OBJ)
				@$(MAKE) --directory $(dir $(MLX)) clean
				@printf $(RED)"Object files removed\n"$(RESET)

fclean:			clean
				@rm -rf $(NAME)
				@$(MAKE) --directory $(dir $(MLX)) fclean
				@printf $(RED)"$(NAME) removed\n"$(RESET)

re:				fclean all

.PHONY:			all clean fclean re run

#text modifiers#
RED =				"\e[31m"
GREEN =				"\e[32m"
YELLOW =			"\e[33m"
BLUE =				"\e[34m"
MAGENTA =			"\e[35m"
CYAN =				"\e[36m"
LIGHTGRAY =			"\e[37m"
DARKGRAY =			"\e[90m"
LIGHTRED =			"\e[91m"
LIGHTGREEN =		"\e[92m"
LIGHTYELLOW =		"\e[93m"
LIGHTBLUE =			"\e[94m"
LIGHTMAGENTA =		"\e[95m"
LIGHTCYAN =			"\e[96m"
RED_BG =			"\e[41m"
GREEN_BG =			"\e[42m"
YELLOW_BG =			"\e[43m"
BLUE_BG =			"\e[44m"
MAGENTA_BG =		"\e[45m"
CYAN_BG =			"\e[46m"
LIGHTGRAY_BG =		"\e[47m"
DARKGRAY_BG =		"\e[100m"
LIGHTRED_BG =		"\e[101m"
LIGHTGREEN_BG =		"\e[102m"
LIGHTYELLOW_BG =	"\e[103m"
LIGHTBLUE_BG =		"\e[104m"
LIGHTMAGENTA_BG =	"\e[105m"
LIGHTCYAN_BG =		"\e[106m"
BOLD =				"\e[1m"
ITALIC =			"\e[3m"
UNDERLINED =		"\e[4m"
RESET =				"\e[0m"
