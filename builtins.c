/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 14:53:09 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/22 22:41:27 by abayar           ###   ########.fr       */
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
	// if (strchr(str, '='))
	// 	return ("");
	return (NULL);
}

// char	*tab_join(char **s)
// {
// 	char	*ret;
// 	int		i;
// 	int		r;

// 	i = 1;
// 	r = 0;
// 	while (s[i])
// 		r += ft_strlen(s[i++]) + 1;
// 	printf("%d", r);
// 	return (ret);
// }

void	export(char **s, t_env **env, t_env **export)
{
	int	i;
	char	*value;
	char	**var;
	t_env	*new;

	i = 1;
	if (tablen(s) == 1)
		printexport(export, s[0]);//printenv(export, s);
	else
	{
		while(s[i])
    	{
			printf("counter --> %d\n", i);
    	    var = ft_split(s[i], '=');
			value = ft_strdup(cut_value(s[i]));
			//printf("name -> %s || value -> %s\n", var[0], cut_value(s[i]));
			if (tablen(var) >= 2)
				new = ft_lstnew(var[0], value);
			else
				new = ft_lstnew(var[0], NULL);
			ft_lstadd_back(export, new);
			// printf("8****  ==   %s   *****\n", new->var_name);
			// if (tablen(var) >= 2)
			// 	ft_lstadd_back(env, new);
			// tab_join(var);
			// while ()
    	    i++;
    	}
	//	printexport(export, s[0]);
	}
}

void	printexport(t_env **export, char *s)
{
	t_env *temp;

	if (ft_strcmp(s, "export") == 0)
	{
		printf("achof okan \n");
		temp = *export;
		if (!temp)
			return ;
		while(temp)
		{
			if (temp->var_value)
				printf("declare -x %s=\"%s\"\n", temp->var_name, temp->var_value);
			else
				printf("declare -x %s\n", temp->var_name);
			temp = temp->next;
		}
	}
}

void	unset(char **s, t_env **env)
{
	t_env *temp;
	t_env *tmp;

	tmp = NULL;
	if (!env)
		return ;
	if (ft_strcmp((*env)->var_name, s[1]) == 0)
	{
		tmp = *env;
		*env = (*env)->next;
		free(tmp->var_name);
		free(tmp->var_value);
		free(tmp);
	}
	temp = *env;
	while(temp->next)
	{
		if (ft_strcmp(temp->next->var_name, s[1]) == 0)
		{
			tmp = temp->next;
			temp->next = temp->next->next;
			free(tmp->var_name);
			free(tmp->var_value);
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