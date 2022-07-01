/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:26:00 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/01 12:27:43 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checker(char **s, char c, int *i)
{
	int	j;

	j = (*i);
	if (tablen(s) <= (*i))
		return (0);
	while (s[j])
	{
		if (s[j][0] == c)
			return (1);
		j++;
	}
	return (0);
}

int	is_degit(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] > '9' || s[i] < '0')
			return (0);
		i++;
	}
	return (1);
}

int	check_exit(char **s)
{
	if (ft_strcmp(s[0], "exit") == 0)
	{
		if (tablen(s) == 2)
		{
			if (is_degit(s[1]))
				g_status = atoi(s[1]);
			else
			{
				perror("exit: numeric argument required");
				g_status = 1;
			}
			exit(g_status);
		}
		else if (tablen(s) > 2)
		{
			perror("exit: too many arguments");
			g_status = 1;
			return (0);
		}
		exit(0);
		return (0);
	}
	return (1);
}
