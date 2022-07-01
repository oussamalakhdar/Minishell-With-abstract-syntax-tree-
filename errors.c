/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 10:31:50 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/01 11:33:07 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_app(char **s, char *s1)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (ft_strcmp(s[i], s1) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	find_space(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' || s[i] != '\t'
			|| s[i] != '\v' || s[i] != '\r' || s[i] != '\b')
			return (0);
		i++;
	}
	return (1);
}

int	checkerrors_extra2(char **s, int i)
{
	if ((s[i][0] == '<' && s[i][1] == '<' && s[i + 1][0] == '<')
		|| (s[i][0] == '<' && s[i][1] == '<' && s[i + 1][0] == '>'))
	{
		perror("minishell: error");
		return (0);
	}
	if ((s[i][0] == '>' && s[i][1] == '>' && s[i][2] == '>')
		|| (s[i][0] == '>' && s[i][1] == '>' && s[i + 1][0] == '<'))
	{
		perror("minishell: error");
		return (0);
	}
	if ((s[i][0] == '>' || s[i][0] == '<') && s[i + 1][0] == '|')
	{
		perror("minishell: error");
		return (0);
	}
	return (1);
}

int	checkerrors_extra1(char **s, int i)
{
	if ((s[i][0] == '>' || s[i][0] == '<') && (tablen(s) == (i + 1)))
	{
		perror("minishell: No file");
		return (0);
	}
	if (s[0][0] == '|' || s[tablen(s) - 1][0] == '|')
	{
		perror("minishell: syntax error");
		return (0);
	}
	if (s[i][0] == '<' && s[i + 1][0] == '>')
	{
		perror("minishell: syntax error");
		return (0);
	}
	if (s[i][0] == '>' && s[i + 1][0] == '<')
	{
		perror("minishell: error");
		return (0);
	}
	return (1);
}

int	checkerrors(char **s)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (*s == NULL)
		return (0);
	if (!check_exit(s))
		return (0);
	while (s[i])
	{
		if (!checkerrors_extra1(s, i))
			return (0);
		if (!checkerrors_extra2(s, i))
			return (0);
		i++;
	}
	return (1);
}
