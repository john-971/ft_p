#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jandreu <jandreu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/09/06 17:45:06 by jandreu           #+#    #+#              #
#    Updated: 2015/09/06 17:49:41 by jandreu          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC=gcc

NAME_CLIENT = client
NAME_SERVER = server
PATH_INC = ./includes/

PATH_INC_LIBFT = ./libft/includes/
INCLUDES = -I $(PATH_INC) -I $(PATH_INC_LIBFT)
CFLAGS =  -Wall -Werror -Wextra $(INCLUDES)

LIBS = -L libft/ -lft -lm

SRC_SERVER = ./serveur/main.c \
			./serveur/server_command.c \
			./serveur/change_dir.c \
			./serveur/file_operation.c \
			./common/send_receive.c \
			./common/create_socket.c \
			./common/output.c \
			./common/file_management.c \
			./common/tools.c \
			./common/manage_errno.c

SRC_CLIENT = ./cli/main.c \
			./cli/cli_command.c \
			./cli/commands.c \
			./cli/parse_msg.c \
			./common/output.c \
			./common/send_receive.c \
			./common/create_socket.c \
			./common/file_management.c \
			./common/tools.c \
			./common/manage_errno.c


OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER): $(OBJ_SERVER)
	make -C libft/
	$(CC) $(OBJ_SERVER) -o $(NAME_SERVER) $(LIBS)
	@echo "\033[36m◉ \033[33mmake server is done\033[0m"

$(NAME_CLIENT): $(OBJ_CLIENT)
	make -C libft/
	$(CC) $(OBJ_CLIENT) -o $(NAME_CLIENT) $(LIBS)
	@echo "\033[36m◉ \033[33mmake client is done\033[0m"

clean:
	rm -f $(OBJ_CLIENT)
	rm -f $(OBJ_SERVER)
	make clean -C libft/
	@echo "\033[36m◉ \033[33mclean done\033[0m"

fclean: clean
	rm -f $(NAME_SERVER)
	rm -f $(NAME_CLIENT)
	make fclean -C libft/
	@echo "\033[36m◉ \033[33mfclean done\033[0m"


re: fclean all

.PHONY: clean fclean re