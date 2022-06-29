/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 15:39:02 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/29 19:52:35 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_path(char **env)
{
	int		i;
	char	**tmp;
	char	**str;

	i = 0;
	while (env[i])
	{
		if (env[i][0] == 'P' && env[i][2] == 'T')
		{
			str = ft_split(env[i], '=');
			tmp = str;
			str = ft_split(str[1], ':');
			free_all(tmp);
			return (str);
		}
		i++;
	}
	str = NULL;
	perror("PATH not found\n");
	exit(1);
}

char	**scan_arg(char **s)
{
	int		j;
	int		count;
	char	**ss;

	j = 0;
	count = 0;
	while (s[j])
	{
		if ((s[j][0] == '>') || (s[j][0] == '<'))
			count++;
		j++;
	}
	ss = malloc(sizeof(char *) * (j - (count * 2) + 1));
	j = 0;
	count = 0;
	while (s[j])
	{
		if (s[j][0] == '<' || s[j][0] == '>')
		{
			j += 2;
			continue ;
		}
		else
			ss[count++] = ft_strdup(s[j++]);
	}
	ss[count] = 0;
	return (ss);
}

int	chrr(char **s, int *i)
{
	while (s[*i])
	{
		if (s[*i][0] == '|')
			return (1);
		(*i)++;
	}
	return (0);
}

int	tablen(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	checker(char **s, char c, int *i)
{
	int	j;

	j = (*i);
	if (tablen(s) <= (*i))
		return (0);
	while (s[j])
	{
		if (s[j][0] == c)
			return (1);
		j++;
	}
	return (0);
}
