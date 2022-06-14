/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 21:37:16 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/14 11:13:52 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*putspace(char *s)
{
	int		i;
	char	*str;
	int		cot;

	i = 0;
	cot = 0;
	str = ft_strdup("");
	s = remove_space(s);
	while (s[i])
	{
		if (s[i + 1] == '|' || (s[i + 1] == '<' && s[i] != '<') || (s[i + 1] == '>' && s[i] != '>'))
		{
		   if (s[i] != ' ')
			{    
				str = charjoin(str, s[i]);
				str = charjoin(str, ' ');
			}
			else
				str = charjoin(str, s[i]);
		}
		else if (s[i] == '|' || s[i] == '<' || s[i] == '>')
		{
			if (s[i + 1] != ' ' && s[i + 1] != '>' && s[i + 1] != '<')
			{
				str = charjoin(str, s[i]);
				str = charjoin(str, ' ');
			}
			else
				str = charjoin(str, s[i]);
		}
		else
			str = charjoin(str, s[i]);
		i++;
	}
	return (str);
}

char	*remove_space(char *s)
{
	int		i;
	char	*str;

	i = 0;
	str = ft_strdup("");
	while(s[i])	
	{
		if (s[i] == '\"')
		{
			str = charjoin(str, s[i++]);
			while(s[i] != '\"')
			{
				if (s[i] == ' ')
					str = charjoin(str, SPACE);
				else if (s[i] == '|')
					str = charjoin(str, PIPE);
				else if (s[i] == '>')
					str = charjoin(str, REDR);
				else if (s[i] == '<')
					str = charjoin(str, REDL);
				else
					str = charjoin(str, s[i]);
				i++;
			}
			str = charjoin(str, s[i]);
		}
		else
			str = charjoin(str, s[i]);
		i++;
	}
	return (str);
}

void	undo(char **s)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i][0] == '\"')
		{
			j = 0;
			while (s[i][j])
			{
				if (s[i][j] == PIPE)
					s[i][j] = '|';
				if (s[i][j] == SPACE)
					s[i][j] = ' ';
				if (s[i][j] == REDR)
					s[i][j] = '>';
				if (s[i][j] == REDL)
					s[i][j] = '<';
				j++;
			}
		}
		i++;
	}
}

// int gettoken(char **start, char *end, char  **fw,char **ew)
// {
// 	char *s;
// 	int ret;

// 	s = *start;
// 	while(
// }