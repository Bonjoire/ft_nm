# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/24 14:34:48 by hubourge          #+#    #+#              #
#    Updated: 2025/02/10 18:23:46 by hubourge         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			= gcc

NAME		= ft_nm

NAME_TEST	= ft_nm_test

CFLAGS		=  -Wall -Wextra -Werror -g

INCLUDE		= -I includes

TEST_MODE	= -D TEST_MODE=1

LIBFT_DIR	= ./libft
LIBFT		= $(LIBFT_DIR)/libft.a

OBJ_DIR		= obj
OBJ_TEST_DIR = obj_test

SRC			=	srcs/main.c \
				srcs/test.c \
				srcs/sort.c \
				srcs/utils.c \
				srcs/parsing.c \
				srcs/parse_elf.c \
				srcs/parse_symbol.c \
				
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

OBJ_TEST	= $(addprefix $(OBJ_TEST_DIR)/, $(SRC:.c=.o))

YELLOW		=\033[0;33m
BIBlue		=\033[1;94m
NC			=\033[0m

all: $(LIBFT) $(OBJ) 
	@ echo "$(YELLOW)Compiling ft_nm...$(NC)"
	@ $(CXX) $(CFLAGS) ${INCLUDE} -o $(NAME) $(OBJ) -L $(LIBFT_DIR) -lft
	@ echo "$(BIBlue)"
	@ echo "   __ _                       "
	@ echo "  / _| |_     _ __  _ __ ___  "
	@ echo " | |_| __|   | '_ \| '_ \` _ \ "
	@ echo " |  _| |_    | | | | | | | | |"
	@ echo " |_|  \__|___|_| |_|_| |_| |_|"
	@ echo "        |_____|               "
	@ echo "                   by hubourge"
	@ echo "$(NC)"

bonus: all

test: $(LIBFT) $(OBJ) $(OBJ_TEST)
	@ echo "$(YELLOW)Compiling ft_nm...$(NC)"
	@ echo "$(YELLOW)Compiling ft_nm_test...$(NC)"
	@ $(CXX) $(CFLAGS) ${INCLUDE} -o $(NAME) $(OBJ) -L $(LIBFT_DIR) -lft
	@ $(CXX) $(CFLAGS) $(TEST_MODE) ${INCLUDE} -o $(NAME_TEST) $(OBJ_TEST) -L $(LIBFT_DIR) -lft
	@ echo "$(BIBlue)"
	@ echo "   __ _                          _            _   "
	@ echo "  / _| |_     _ __  _ __ ___    | |_ ___  ___| |_ "
	@ echo " | |_| __|   | '_ \| '_ \` _ \   | __/ _ \/ __| __|"
	@ echo " |  _| |_    | | | | | | | | |  | ||  __/\__ \ |_ "
	@ echo " |_|  \__|___|_| |_|_| |_| |_|___\__\___||___/\__|"
	@ echo "        |_____|             |_____|               "
	@ echo "                   by hubourge"
	@ echo "$(NC)"

$(LIBFT):
	@ echo "$(YELLOW)Compiling libft...$(NC)"	
	@ make -sC $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c
	@ mkdir -p $(@D)
	@ $(CXX) $(CFLAGS) ${INCLUDE} -c $< -o $@

$(OBJ_TEST_DIR)/%.o: %.c
	@ mkdir -p $(@D)
	@ $(CXX) $(CFLAGS) $(TEST_MODE) ${INCLUDE} -c $< -o $@

clean:
	@ echo "$(YELLOW)Cleaning in progress...$(NC)"
	@ rm -rf $(OBJ) $(OBJ_TEST) obj obj_test test
	@ make -sC $(LIBFT_DIR) clean
	@ echo "$(YELLOW)Cleaning completed$(NC)\n"

fclean: clean
	@ rm -rf $(NAME) $(NAME_TEST)
	@ make -sC $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re test bonus