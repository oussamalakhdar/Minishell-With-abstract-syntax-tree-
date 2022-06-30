/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 16:08:04 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/30 20:08:24 by abayar           ###   ########.fr       */
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
				printf("declare -x %s=\'\'\n", temp->var_name);
			else
				printf("declare -x %s\n", temp->var_name);
			temp = temp->next;
		}
	}
}

void	unset_helper(t_env *temp, t_env *tmp, char *s)
{
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
		unset_helper(temp, tmp, s);
		temp = temp->next;
	}
}
