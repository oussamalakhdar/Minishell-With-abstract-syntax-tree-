/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 14:53:09 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/28 17:44:42 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*pwd(void)
{
	char	pwd[1000];

	getcwd(pwd, sizeof(pwd));
	return (ft_strdup(pwd));
}

void	echo(char **s)
{
	int	i;
	int	c;

	c = 0;
	if (ft_strcmp(s[1], "-n") == 0)
	{
		c = 1;
		i = 2;
	}
	else
		i = 1;
	while (s[i])
	{
		printf("%s", s[i]);
		if (i + 1 != tablen(s))
			printf(" ");
		i++;
	}
	if (c == 0)
		printf("\n");
	exit(0);
}

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
		printexport(export, s[0]);//printenv(export, s);
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

void	printexport(t_env **export, char *s)
{
	t_env	*temp;

	if (ft_strcmp(s, "export") == 0)
	{
		temp = *export;
		if (!temp)
			return ;
		temp = temp->next;
		while (temp)
		{
			if (temp->var_name[0] == '\0')
				return ;
			if (temp->var_value)
				printf("declare -x %s=\"%s\"\n", temp->var_name, temp->var_value);
			else if (temp->flag)
				printf("declare -x %s=\"\"\n", temp->var_name);
			else
				printf("declare -x %s\n", temp->var_name);
			temp = temp->next;
		}
	}
}

void	unset(char *s, t_env **env)
{
	t_env	*temp;
	t_env	*tmp;

	tmp = NULL;
	if (!env || !s)
		return ;
	if (ft_strcmp((*env)->var_name, s) == 0)
	{
		tmp = *env;
		*env = (*env)->next;
		free(tmp->var_name);
		free(tmp->var_value);
		tmp->flag = 0;
		free(tmp);
	}
	temp = *env;
	while (temp)
	{
		if (!temp->next)
			break ;
		if (ft_strcmp(temp->next->var_name, s) == 0)
		{
			tmp = temp->next;
			if (temp->next->next)
				temp->next = temp->next->next;
			else
				temp->next = NULL;
			free(tmp->var_name);
			free(tmp->var_value);
			tmp->flag = 0;
			tmp->next = NULL;
			free(tmp);
			tmp = NULL;
		}
		temp = temp->next;
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

void	builtins(char **s, t_env **env, t_env **exportt)
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
