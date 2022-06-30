/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 15:53:44 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/30 17:36:55 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	helper_scanner(char *s, char q, int *i)
{
	if (s[(*i)] == q)
	{
		(*i)++;
		while (s[(*i)] != q)
		{
			if (s[(*i)] == '\0')
			{
				perror("wa sed hadik quote ra mhandlinha");
				return (-1);
			}
			(*i)++;
		}
	}
	return (0);
}

int	scanner(char *s)
{
	int	i;
	int	d;

	i = 0;
	d = 0;
	while (s[i])
	{
		if (helper_scanner(s, '\"', &i) == -1)
			return (-1);
		if (helper_scanner(s, '\'', &i) == -1)
			return (-1);
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

	i = -1;
	while (s[++i])
	{
		j = -1;
		while (s[i][++j])
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
		}
	}
}
