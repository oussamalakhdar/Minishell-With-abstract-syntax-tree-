/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 14:53:09 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/30 19:58:09 by abayar           ###   ########.fr       */
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

void	free_wkriw(char **var, t_env *new, t_env *new2)
{
	free_all(var);
	if (new->var_name)
		free(new->var_name);
	if (new->var_value)
		free(new->var_value);
	free(new);
	new = NULL;
	if (new2->var_name)
		free(new2->var_name);
	if (new2->var_value)
		free(new2->var_value);
	free(new2);
	new2 = NULL;
}

void	export_helper(t_ex *data, t_env **env, char *s, t_env **export)
{
	if (tablen(data->var) >= 2)
	{
		data->new = ft_lstnew(ft_strdup(data->var[0]), ft_strdup(data->value));
		data->new2 = ft_lstnew(ft_strdup(data->var[0]), ft_strdup(data->value));
		if (scan_list(data->var[0], env))
			unset(data->var[0], env);
		ft_lstadd_back(env, data->new);
	}
	else
	{
		data->new = ft_lstnew(ft_strdup(data->var[0]), NULL);
		data->new2 = ft_lstnew(ft_strdup(data->var[0]), NULL);
		if (ft_strchr(s, '=') != -1)
		{
			data->new2->flag = 1;
			if (scan_list(data->var[0], env))
				unset(data->var[0], env);
			ft_lstadd_back(env, data->new);
		}
	}
	if (strchr(s, '='))
	{
		if (scan_list(data->var[0], export))
			unset(data->var[0], export);
	}
}

void	export(char **s, t_env **env, t_env **export)
{
	int		i;
	t_ex	data;

	i = 0;
	if (tablen(s) == 1)
		printexport(export, s[0]);
	else
	{
		if (is_specialchar(s[1][0]) || (s[1][0] >= '0' && s[1][0] <= '9'))
		{
			perror("export failed");
			return ;
		}
		while (s[++i])
		{
			data.var = ft_split(s[i], '=');
			data.value = cut_value(s[i]);
			scan_list(data.var[0], export);
			export_helper(&data, env, s[i], export);
			if (!scan_list(data.var[0], export))
				ft_lstadd_back(export, data.new2);
			free_wkriw(data.var, data.new, data.new2);
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
