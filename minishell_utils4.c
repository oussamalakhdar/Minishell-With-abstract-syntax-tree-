/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:29:44 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 15:34:52 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_utils2(char *line)
{
	if (!line)
		return (0);
	if (ft_strncmp(line, "\n", ft_strlen(line)))
	{
		free(line);
		line = NULL;
		return (1);
	}
	if (line && find_space(line))
	{
		free(line);
		line = NULL;
		return (1);
	}
	add_history(line);
	return (-1);
}

void	handlle(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

char	*find(char *ss, char c)
{
	int	i;

	i = 0;
	while (ss[i])
	{
		if (ss[i] == c || ss[i] == '\'' || ss[i] == '\"' || ss[i] == '\n')
			ss[i] = '\0';
		i++;
	}
	return (ss);
}

void	rednorm(t_redir *cmdd, char *s)
{
	cmdd->infd = open(s, O_RDWR, 0777);
	cmdd->herd = 1;
}

char	**parceline(char **s, int *i)
{
	char	**n;
	int		j;
	int		tmp;
	int		k;

	j = 0;
	tmp = (*i);
	if (s[*i] == NULL || tablen(s) <= (*i))
		return (NULL);
	while (s[*i])
	{
		if (s[*i][0] == '|')
			break ;
		(*i)++;
	}
	n = malloc(sizeof(char *) * ((*i) - tmp) + 1);
	k = (*i) - tmp;
	while (j < k)
	{
		n[j++] = ft_strdup(s[tmp]);
		tmp++;
	}
	n[j] = 0;
	(*i)++;
	return (n);
}
