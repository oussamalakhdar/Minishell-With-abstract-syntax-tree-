/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 12:08:52 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/22 22:32:12 by abayar           ###   ########.fr       */
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

t_env	*ft_lstnew(char *name, char *value)
{
	t_env	*ptr;

	ptr = malloc(sizeof(t_env));
	if (ptr == NULL)
		return (NULL);
	ptr->var_name = name;
	ptr->var_value = value;
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