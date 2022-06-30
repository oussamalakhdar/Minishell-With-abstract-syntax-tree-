/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 14:53:09 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/30 11:20:26 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cut_value(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			i++;
			return (&str[i]);
		}
		i++;
	}
	return (NULL);
}

void	export(char **s, t_env **env, t_env **export)
{
	int		i;
	char	*value;
	char	**var;
	t_env	*new;
	t_env	*new2;

	i = 1;
	if (tablen(s) == 1)
		printexport(export, s[0]);
	else
	{
		while (s[i])
		{
			var = ft_split(s[i], '=');
			value = cut_value(s[i]);
			scan_list(var[0], export);
			if (tablen(var) >= 2)
			{
				new = ft_lstnew(var[0], ft_strdup(value));
				new2 = ft_lstnew(ft_strdup(var[0]), ft_strdup(value));
				if (scan_list(var[0], env))
					unset(var[0], env);
				ft_lstadd_back(env, new);
			}
			else
			{
				new = ft_lstnew(var[0], NULL);
				new2 = ft_lstnew(var[0], NULL);
				if (ft_strchr(s[i], '=') != -1)
				{
					new2->flag = 1;
					if (scan_list(var[0], env))
						unset(var[0], env);
					ft_lstadd_back(env, new);
				}
			}
			if (strchr(s[i], '='))
			{
				if (scan_list(var[0], export))
					unset(var[0], export);
			}
			if (!scan_list(var[0], export))
				ft_lstadd_back(export, new2);
			i++;
		}
	}
}

char	check_cmd(char *s)
{
	if (ft_strcmp(s, "echo") == 0)
		return ('e');
	if (ft_strcmp(s, "pwd") == 0)
		return ('p');
	if (ft_strcmp(s, "unset") == 0)
		return ('u');
	if (ft_strcmp(s, "export") == 0)
		return ('x');
	if (ft_strcmp(s, "env") == 0)
		return ('v');
	return ('a');
}

void	builtins(char **s, t_env **env)
{
	char	c;

	c = check_cmd(s[0]);
	if (c == 'a')
		return ;
	if (c == 'p')
	{
		printf("%s\n", pwd());
		exit(0);
	}
	if (c == 'e')
		echo(s);
	if (c == 'v')
		printenv(env, s[0]);
}
