/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 21:37:16 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/20 14:50:11 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	scanner(char *s)
{
	int	i;
	int	ss;
	int	d;

	i = 0;
	ss = 0;
	d = 0;
	while (s[i])
	{
		if (s[i] == '\"')
		{
			if (d == 0)
				d = 1;
			else
				d = 0;
			//i++;
			// if (ss == 1)
			// 	perror("no such file or directory");
		}
		if (s[i] == '\'')
		{
			if (ss == 0)
				ss = 1;
			else
				ss = 0;
			//i++;
			// if (d == 1)
			// 	perror("no such file or directory");
		}
		//printf("i -->  %d  &&&  dd = %d  &&& s = %d\n", i, d, ss);
		i++;
	}
	if (d == 1 || ss == 1)
	{
		perror("hadchi machi me3qol");
		return (-1);
	}
	return (0);
}

char	*putspace(char *s)
{
	int		i;
	int		j;
	char	*str;
	int		cot;

	i = 0;
	cot = 0;
	str = ft_strdup("");
	if (scanner(s) == -1)
		return (NULL);
	while (i <= ft_strlen(s))
	{
		//printf("%s     %d\n",s, i);
		s = remove_space(s, '\'', &i);
		i = 0;
		s = remove_space(s, '\"', &i);
		i++;
	}
	i = 0;
	//printf("----%s------\n",s);
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

char	*remove_space(char *s,char c, int *i)
{
	char	*str;

	str = ft_strdup("");
	if ((*i) >= ft_strlen(s))
		return (s);
	while(s[(*i)])	
	{
		if (s[(*i)] == c)
		{
			(*i)++;
			while(s[(*i)] != c && s[(*i)])
			{
				if (c == '\"' && s[(*i)] == '\'')
				{
					perror("syntax error");
					return (NULL);
				}
				else if (s[(*i)] == '\"')
				{
					perror("syntax error");
					return (NULL);
				}
				if (s[(*i)] == ' ')
					str = charjoin(str, SPACE);
				else if (s[(*i)] == '|')
					str = charjoin(str, PIPE);
				else if (s[(*i)] == '>')
					str = charjoin(str, REDR);
				else if (s[(*i)] == '<')
					str = charjoin(str, REDL);
				else
					str = charjoin(str, s[(*i)]);
				(*i)++;
			}
			//return (str);
		}
		else
			str = charjoin(str, s[(*i)]);
		(*i)++;
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
		i++;
	}
}
