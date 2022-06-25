# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abayar <abayar@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/25 13:43:57 by abayar            #+#    #+#              #
#    Updated: 2022/06/25 13:44:00 by abayar           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc -g

# READLINE = -lreadline -I/Users/abayar/goinfre/abayar/.brew/Cellar/readline/8.1.2/include \
# 			-L/Users/abayar/goinfre/abayar/.brew/Cellar/readline/8.1.2/lib \

READLINE = -lreadline -L/goinfre/abayar/abayar/.brew/opt/readline/lib -I/goinfre/abayar/abayar/.brew/opt/readline/include

FLAGS =  $(READLINE)

FILES = minishell.c ft_split.c lists.c minishell_utils.c lexer.c getnextline/get_next_line.c getnextline/get_next_line_utils.c \
		builtins.c \

# OBJ=$(FILES:.c=.o)

all: $(NAME)

$(NAME):
	@$(CC) $(FLAGS) $(FILES) -o $(NAME)

clean:
	@rm -f

fclean: clean
	@rm -rf $(NAME)

re: fclean all
