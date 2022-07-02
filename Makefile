# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/26 14:33:38 by olakhdar          #+#    #+#              #
#    Updated: 2022/07/02 15:32:40 by olakhdar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = gcc  -Wall -Werror -Wextra 

READLINE = -lreadline -L/goinfre/olakhdar/olakhdar/.brew/opt/readline/lib -I/goinfre/olakhdar/olakhdar/.brew/opt/readline/include

FLAGS =  $(READLINE)

FILES = minishell.c ft_split.c lists.c minishell_utils.c lexer.c getnextline/get_next_line.c getnextline/get_next_line_utils.c \
		builtins.c ft_itoa.c minishell_utils1.c minishell_utils2.c minishell_utils3.c  minishell_utils4.c \
		builtins_extra.c magic_time.c parce.c errors.c exit.c lexer_utils.c lexer_utils2.c \
		execution.c execution_utils1.c execution_utils2.c free_three.c get_files.c \
# OBJ=$(FILES:.c=.o)

all: $(NAME)

$(NAME):
	@$(CC) $(FLAGS) $(FILES) -o $(NAME)

clean:
	@rm -f

fclean: clean
	@rm -rf $(NAME)

re: fclean all