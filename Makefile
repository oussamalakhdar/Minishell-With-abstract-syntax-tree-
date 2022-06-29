# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abayar <abayar@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/26 14:33:38 by olakhdar          #+#    #+#              #
#    Updated: 2022/06/29 18:43:59 by abayar           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = gcc -g3# -Wall -Werror -Wextra -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include

READLINE = -lreadline -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include

FLAGS =  $(READLINE)

FILES = minishell.c ft_split.c lists.c minishell_utils.c lexer.c getnextline/get_next_line.c getnextline/get_next_line_utils.c \
		builtins.c ft_itoa.c minishell_utils1.c minishell_utils2.c minishell_utils3.c \
		builtins_extra.c \

# OBJ=$(FILES:.c=.o)

all: $(NAME)

$(NAME):
	@$(CC) $(FLAGS) $(FILES) -o $(NAME)

clean:
	@rm -f

fclean: clean
	@rm -rf $(NAME)

re: fclean all
