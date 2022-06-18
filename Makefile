# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abayar <abayar@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/02 10:46:18 by olakhdar          #+#    #+#              #
#    Updated: 2022/06/18 14:42:33 by abayar           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

FLAGS =  -Wall -Werror -Wextra -lreadline -g #-fsanitize=address

FILES = minishell.c ft_split.c lists.c minishell_utils.c lexer.c getnextline/get_next_line.c getnextline/get_next_line_utils.c \

OBJ=$(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
