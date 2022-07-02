/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:33:51 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 14:35:32 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_value_utils(char *s, int *i, char *ret)
{
	if (s[(*i)] == '\'' \
		|| ((*i) > 1 && s[(*i) - 1] == '<' && s[(*i) - 2] == '<'))
	{
		if (s[(*i) - 1] != '<')
		{
			(*i)++;
			while (s[(*i)] != '\'' && s[(*i)])
			{
				ret = charjoin(ret, s[(*i)]);
				(*i)++;
			}
		}
		else
		{
			while (s[(*i)] && (s[(*i)] == SPACE2 || s[(*i)] == ' '))
				(*i)++;
			while (s[(*i)] && s[(*i)] != SPACE2 && s[(*i)] != ' ')
			{
				ret = charjoin(ret, s[(*i)]);
				(*i)++;
			}
		}
	}
}

void	add_value_utils2(char *s, int i)
{
	if (s[i] == '\"')
	{
		i++;
		while (s[i] && s[i] != '\"')
		{
			if (s[i] == '\'')
				s[i] = QUOT;
			i++;
		}
	}
}

int	add_value_utils3(char *s, int i, char **ret)
{
	if (s[i] == '\'' || (i > 1 && s[i - 1] == '<' && s[i - 2] == '<'))
	{
		if (s[i - 1] != '<')
		{
			*ret = charjoin(*ret, s[i]);
			i++;
			while (s[i] != '\'' && s[i])
			{
				if (s[i] == '\"')
					*ret = charjoin(*ret, QUOT2);
				else
					*ret = charjoin(*ret, s[i]);
				i++;
			}
		}
		else
		{
			while (s[i] && (s[i] == SPACE2 || s[i] == ' '))
				i++;
			while (s[i] && s[i] != SPACE2 && s[i] != ' ')
				*ret = charjoin(*ret, s[i++]);
		}
	}
	return (i);
}

int	add_value_utils4(char **name, char **ret, char *s, int i)
{
	char	*stat;

	if (s[i + 1] == '?')
	{
		*name = ft_strdup(s);
		(*name)[i] = '\0';
		stat = ft_itoa(g_status);
		free(*ret);
		*ret = ft_strjoin(*name, stat);
		free(stat);
		return (0);
	}
	return (1);
}

void	add_value_utils5(char *name)
{
	int	p;

	p = 0;
	while (name[p])
	{
		if (is_specialchar(name[p]))
			name[p] = '\0';
		p++;
	}
}
