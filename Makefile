# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/26 14:33:38 by olakhdar          #+#    #+#              #
#    Updated: 2022/06/28 18:25:20 by olakhdar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = gcc -g

# READLINE = -lreadline -I/Users/abayar/goinfre/abayar/.brew/Cellar/readline/8.1.2/include \
# 			-L/Users/abayar/goinfre/abayar/.brew/Cellar/readline/8.1.2/lib \

READLINE = -lreadline -L/goinfre/olakhdar/olakhdar/.brew/opt/readline/lib -I/goinfre/olakhdar/olakhdar/.brew/opt/readline/include

FLAGS =  $(READLINE)

FILES = minishell.c ft_split.c lists.c minishell_utils.c lexer.c getnextline/get_next_line.c getnextline/get_next_line_utils.c \
		builtins.c ft_itoa.c minishell_utils1.c \

# OBJ=$(FILES:.c=.o)

all: $(NAME)

$(NAME):
	@$(CC) $(FLAGS) $(FILES) -o $(NAME)

clean:
	@rm -f

fclean: clean
	@rm -rf $(NAME)

re: fclean all
