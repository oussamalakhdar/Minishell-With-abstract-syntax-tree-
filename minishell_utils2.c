/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 15:39:02 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/01 11:31:25 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_path(char **env, t_env *envp)
{
	int		i;
	char	**tmp;
	char	**str;

	i = 0;
	if (!scan_list("PATH", &envp))
		return (NULL);
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

int	count_file(char **s, int *j)
{
	int	count;

	count = 0;
	while (s[(*j)])
	{
		if ((s[(*j)][0] == '>') || (s[(*j)][0] == '<'))
			count++;
		(*j)++;
	}
	return (count);
}

char	**scan_arg(char **s)
{
	int		j;
	int		count;
	char	**ss;

	j = 0;
	count = 0;
	count = count_file(s, &j);
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
