/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 10:20:26 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/01 15:45:08 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handlle(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
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

char	*read_f_utils2(char *ss, t_env **env, char *str, int *i)
{
	char	*value;
	char	*newstr;
	int		j;

	j = 0;
	value = scan_list(ss, env);
	newstr = ft_strdup("");
	while (*i < ft_strlen(str))
	{
		if (str[(*i)] == '$')
		{
			while (value[j])
				newstr = charjoin(newstr, value[j++]);
			while (str[(*i)] && str[(*i)] != ' ' \
				&& str[(*i)] != '\'' && str[(*i)] != '\"')
				(*i)++;
			(*i)--;
			if (str[(*i)] == '\n')
				newstr = charjoin(newstr, '\n');
		}
		else
			newstr = charjoin(newstr, str[(*i)]);
		(*i)++;
	}
	return (newstr);
}

char	*read_f_utils(char *str, t_env **env, int *i)
{
	char	*newstr;
	char	*ss;

	newstr = NULL;
	if (find_dollar(str))
	{
		newstr = ft_strdup("");
		ss = ft_strdup(find_dollar(str));
		ss = find(ss, ' ');
		if (scan_list(ss, env))
			newstr = read_f_utils2(ss, env, str, i);
		else if (scan_list(ss, env) == NULL)
		{
			(*i)++;
			newstr = ft_strdup(find(str, '$'));
			if (ss[0] == '\0')
				newstr = charjoin(newstr, '$');
			newstr = charjoin(newstr, '\n');
		}
	}
	return (newstr);
}

void	read_file(char **str, t_env **env, int fd, char *s)
{
	char	*newstr;
	char	*tmp;
	int		i;
	int		l;

	l = ft_strlen(s);
	newstr = NULL;
	tmp = *str;
	while (1)
	{
		i = 0;
		newstr = read_f_utils(*str, env, &i);
		if (i > 0)
			write(fd, newstr, ft_strlen(newstr));
		else
			write(fd, *str, ft_strlen(*str));
		free(*str);
		*str = NULL;
		write(1, "heredoc> ", 9);
		*str = get_next_line(0);
		if (*str == NULL || (ft_strncmp(s, *str, l) == 1
				&& *str[l] == '\n'))
			break ;
	}
}

void	read_f(char *s, int fd, t_env **env)
{
	int		i;
	int		j;
	char	*str;
	int		l;
	char	*newstr;

	i = 0;
	j = 0;
	str = NULL;
	if (!s)
		return ;
	l = ft_strlen(s);
	write(1, "heredoc> ", 9);
	str = get_next_line(0);
	if (str == NULL || (ft_strncmp(s, str, l) == 1 && str[l] == '\n'))
		;
	else
	{
		while (1)
		{
			i = 0;
			newstr = read_f_utils(str, env, &i);
			if (i > 0)
				write(fd, newstr, ft_strlen(newstr));
			else
				write(fd, str, ft_strlen(str));
			free(str);
			str = NULL;
			write(1, "heredoc> ", 9);
			str = get_next_line(0);
			if (str == NULL || (ft_strncmp(s, str, l) == 1
					&& str[l] == '\n'))
				break ;
		}
	}
	close_read_f(fd, str);
}
