/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 12:08:52 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/23 12:09:42 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}


// int	chrbool(t_env **export, char *s)
// {
// 	t_env	*tmp;

// 	tmp = *export;
// 	while (tmp)
// 	{
// 		if (ft_strcmp() == 0)
// 		tmp = tmp->next;
// 	}
// }

int	scan_list(char **s, t_env **node)
{
	t_env	*new;

	new = *node;
	while (new)
	{
		if (ft_strcmp(new->var_name, s[0]) == 0)
		{
			return (1);
		}	
		new = new->next;
	}
	return (0);
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

	temp = *lst;
	//printf("8****  ==   %s   *****\n", new->var_name);
	if ((*lst) == NULL)
	{
		*lst = new;
		(*lst)->next = NULL;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

void	createnv(t_env **env, char **envp)
{
    int     i;
    t_env   *new;
    char **var;

    i = 0;
	if (!envp)
		return ;
    while(envp[i])
    {
        var = ft_split(envp[i], '=');
		if (ft_strcmp(var[0], "PWD") == 0)
        	new = ft_lstnew(var[0], pwd());
		else	
        	new = ft_lstnew(var[0], var[1]);
        ft_lstadd_back(env, new);
        i++;
    }
}

int	ft_lstsize(t_env *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
