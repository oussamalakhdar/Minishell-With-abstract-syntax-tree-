/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 14:53:09 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/22 11:05:25 by olakhdar         ###   ########.fr       */
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

void	export(char **s, t_env **env, t_env **export)
{
	int	i;
	char	**var;
	t_env	*new;

	i = 1;
	if (tablen(s) == 1)
		printexport(export, s[0]);//printenv(export, s);
	else
	{
		while(s[i])
    	{
    	    var = ft_split(s[i], '=');	
    	    new = ft_lstnew(var[0], var[1]);
    	    ft_lstadd_back(env, new);
    	    i++;
    	}
	}
}

void	printexport(t_env **export, char *s)
{
	t_env *temp;

	if (ft_strcmp(s, "export") == 0)
	{
		temp = *export;
		if (!temp)
			return ;
		while(temp)
		{
			printf("%s=%s\n", temp->var_name, pwd());
			temp = temp->next;
		}
	}
}

// void	unset(char **s, t_env **env)
// {
// 	t_env *temp;
// 	t_env *tmp;

// 	temp = *env;
// 	tmp = NULL;
// 	if (!temp)
// 		return ;
// 	if (ft_strcmp((*env)->var_name, s[1]) == 0)
// 	{
// 		tmp = *env;
// 		*env = (*env)->next;
// 		free(tmp->var_name);
// 		free(tmp->var_value);
// 		free(tmp);
// 	}
// 	// while(temp)
// 	// {
		
// 	// 	if (strcmp(temp->next->var_name, s[1]) == 0)
// 	// 	{
// 	// 		tmp =
// 	// 	}
// 	// 	temp = temp->next;
// 	// }
// }

void	unset(char **s, t_env **env)
{
	// t_env *temp;
	t_env *tmp;

	tmp = NULL;
	if (!env)
		return ;
	if (ft_strcmp((*env)->var_name, s[1]) == 0)
	{
		tmp = *env;
		*env = (*env)->next;
		printf("*1var name  = %s *** var value %s*********\n", tmp->var_name, tmp->var_value);
		free(tmp->var_name);
		free(tmp->var_value);
		tmp->next = NULL;
		free(tmp);
		printf("*1*********%s*********\n", (*env)->var_name);
		printf("*2*********%s*********\n", (*env)->var_value);
		printenv(env, "env");
	}
	// return (env);
	// temp = *env;
	// while(temp->next)
	// {
	// 	if (ft_strcmp(temp->next->var_name, s[1]) == 0)
	// 	{
	// 		printf("3**********1 ==%s*********\n", temp->next->var_name);
	// 		tmp = temp->next;
	// 		temp->next = temp->next->next;
	// 		printf("*4*********2 ==%s*********\n", temp->next->var_name);
	// 		free(tmp->var_name);
	// 		free(tmp->var_value);
	// 		tmp->next = NULL;
	// 		free(tmp);
	// 		printf("***5*******%s*********\n", tmp->var_name);
	// 		printf("***6*******%s*********\n", tmp->var_value);
	// 		tmp = NULL;
	// 	}
	// 	temp = temp->next;
	// }
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
		printenv(env, s[0]);;
	if (c == 'u')
	{	
		unset(s, env);
		exit(0);
	}
	if (c == 'x')
	{
		export(s, env, exportt);
		exit(0);
	}	
}