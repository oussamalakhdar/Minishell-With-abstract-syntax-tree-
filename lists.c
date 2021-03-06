/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 12:08:52 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/01 09:56:50 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(char **s)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

char	*scan_list(char *s, t_env **node)
{
	t_env	*new;
	int		i;

	i = 0;
	new = *node;
	while (new)
	{
		if (ft_strcmp(new->var_name, s) == 0)
		{
			if (!new->var_value)
				return ("");
			return (new->var_value);
		}	
		new = new->next;
	}
	return (NULL);
}

t_env	*ft_lstnew(char *name, char *value)
{
	t_env	*ptr;

	ptr = malloc(sizeof(t_env));
	if (ptr == NULL)
		return (NULL);
	ptr->var_name = name;
	ptr->var_value = value;
	ptr->flag = 0;
	ptr->next = NULL;
	return (ptr);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*temp;
	t_env	*new2;

	temp = *lst;
	new2 = ft_lstnew(ft_strdup(new->var_name), ft_strdup(new->var_value));
	if ((*lst) == NULL)
	{
		*lst = new2;
		(*lst)->next = NULL;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new2;
}

void	createnv(t_env **env, char **envp)
{
	int		i;
	t_env	*new;
	char	**var;
	char	*pwdd;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		var = NULL;
		var = ft_split(envp[i], '=');
		if (ft_strcmp(var[0], "PWD") == 0)
		{
			pwdd = pwd();
			new = ft_lstnew(var[0], pwdd);
			free(pwdd);
		}
		else
			new = ft_lstnew(var[0], var[1]);
		ft_lstadd_back(env, new);
		free_all(var);
		free(new);
		i++;
	}
}
