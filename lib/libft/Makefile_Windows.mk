# Commands & Flags
CC = gcc
CCFLAGS = -Wall -Werror -Wextra

RM = rm

AR = ar
ARFLAGS = -crs

# Locations
SRC_IO = \
	ft_printf.c ft_putchar_fd.c ft_putchar.c ft_putendl_fd.c ft_putlhex.c ft_putlnbr.c \
	ft_putnbr.c ft_putptr.c ft_putstr_fd.c ft_putstr.c ft_putuhex.c ft_putunbr.c \
	ft_puterror.c ft_putstderr.c ft_putnbr_fd.c
SRC_LST = \
	ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c \
	ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c
SRC_MEM = \
	ft_bzero.c ft_calloc.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c
SRC_RW = \
	get_next_line.c ft_get_line_count.c
SRC_STRING = \
	ft_append.c ft_atoi.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
	ft_itoa.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c \
	ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strnstr.c ft_strncmp.c ft_strrchr.c ft_strstr.c ft_strtrim.c \
	ft_substr.c ft_tolower.c ft_toupper.c ft_strhas.c ft_strndup.c ft_htoa.c ft_getlongeststr.c \
	ft_atol.c ft_isspace.c ft_chrcount.c ft_issign.c ft_strcmp.c ft_str_arr_len.c ft_split_set.c
SRC_UTILS = \
	ft_free.c ft_hexnbrlen.c ft_nbrlen.c ft_free_split.c

OBJ_DIR = ./obj/

OBJ_IO = $(addprefix $(OBJ_DIR), $(SRC_IO:.c=.o))
OBJ_LST = $(addprefix $(OBJ_DIR), $(SRC_LST:.c=.o))
OBJ_MEM = $(addprefix $(OBJ_DIR), $(SRC_MEM:.c=.o))
OBJ_RW = $(addprefix $(OBJ_DIR), $(SRC_RW:.c=.o))
OBJ_STRING = $(addprefix $(OBJ_DIR), $(SRC_STRING:.c=.o))
OBJ_UTILS = $(addprefix $(OBJ_DIR), $(SRC_UTILS:.c=.o))

HEADER = ./include/libft.h

NAME = ./lib/libft.a

Y = \033[33m
R = \033[31m
G = \033[32m
B = \033[34m
X = \033[0m
UP = \033[A
CUT = \033[K

all: $(NAME)

$(NAME): $(OBJ_IO) $(OBJ_LST) $(OBJ_MEM) $(OBJ_RW) $(OBJ_STRING) $(OBJ_UTILS)
	@if not exist lib/ md lib
	@$(AR) $(ARFLAGS) $(NAME) $(OBJ_IO) $(OBJ_LST) $(OBJ_MEM) $(OBJ_RW) $(OBJ_STRING) $(OBJ_UTILS)
	@if not exist .\libft.a copy .\lib\libft.a .
	@echo $(notdir $(NAME)) created

$(OBJ_DIR)%.o: ./src/*/%.c
	@if not exist .\obj\\ md .\obj\\
	@$(CC) $(CCFLAGS) -I ./include -o $@ -c $<
	@echo $(notdir $<) compiled

clean:
	@if exist .\obj\\ del /S /Q .\obj\\
	@echo $(notdir $(NAME)) object files removed

fclean: clean
	@if exist .\lib\libft.a del .\lib\libft.a
	@if exist .\libft.a del .\libft.a
	@echo $(notdir $(NAME)) removed

re: fclean all

.PHONY: all clean fclean re