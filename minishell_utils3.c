/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 15:53:44 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/30 15:10:20 by abayar           ###   ########.fr       */
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
	//printf("--------%s\n",s);
	while (s[i])
	{
		
		if (s[i] == '\"')
		{
			i++;
			while (s[i] != '\"')
			{
				if (s[i] == '\0')
				{
					perror("wa sed hadik quote ra mhandlinha");
					return(-1);
				}
				i++;
			}
		}
		if (s[i] == '\'')
		{
			i++;
			while (s[i] != '\'')
			{
				if (s[i] == '\0')
				{
					perror("wa sed hadik quote ra mhandlinha");
					return(-1);
				}
				i++;
			}
		}
		i++;
	}
	return (0);
}

int	is_specialchar(char s)
{
	if (s == '_')
		return (0);
	if (s < 'A' || (s > 'Z' && s < 'a') || s > 'z')
	{
		if (s >= '0' && s <= '9')
			return (0);
		return (1);
	}
	return (0);
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
			if (s[i][j] == SPACE2)
				s[i][j] = ' ';
			if (s[i][j] == REDR)
				s[i][j] = '>';
			if (s[i][j] == REDL)
				s[i][j] = '<';
			if (s[i][j] == QUOT)
				s[i][j] = '\'';
			if (s[i][j] == QUOT2)
				s[i][j] = '\"';
			if (s[i][j] == NL)
				s[i][j] = '\0';
			j++;
		}
		i++;
	}
}
