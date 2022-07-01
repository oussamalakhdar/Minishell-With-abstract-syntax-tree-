/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 16:31:18 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/01 16:36:41 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_dollar(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			return (&s[i + 1]);
		i++;
	}
	return (NULL);
}

char	*putspace_utils(char *s, char *str, int i)
{
	if (s[i + 1] == '|' || (s[i + 1] == '<' && s[i] != '<') \
		|| (s[i + 1] == '>' && s[i] != '>'))
	{
		if (s[i] != ' ')
		{
			str = charjoin(str, s[i]);
			str = charjoin(str, ' ');
		}
		else
			str = charjoin(str, s[i]);
	}
	else if (s[i] == '|' || s[i] == '<' || s[i] == '>')
	{
		if (s[i + 1] != ' ' && s[i + 1] != '>' && s[i + 1] != '<')
		{
			str = charjoin(str, s[i]);
			str = charjoin(str, ' ');
		}
		else
			str = charjoin(str, s[i]);
	}
	else
		str = charjoin(str, s[i]);
	return (str);
}

char	*putspace(char *s, t_env **env)
{
	int		i;
	char	*str;
	int		cot;

	i = 0;
	cot = 0;
	str = ft_strdup("");
	while (i <= ft_strlen(s))
	{
		s = add_value(s, env);
		if (scanner(s) == -1)
		{
			g_status = 1;
			return (NULL);
		}
		s = remove_space(s, '\"', &i);
		i = 0;
		s = remove_space(s, '\'', &i);
		i++;
	}
	i = 0;
	while (s[i])
		str = putspace_utils(s, str, i++);
	free(s);
	return (str);
}

char	*remove_space_utils(char *s, char c, char *str, int i)
{
	if (c == '\"' && s[i] == '\'')
		str = charjoin(str, QUOT);
	if (c == '\'' && s[i] == '\"')
		str = charjoin(str, QUOT2);
	if (s[i] == ' ')
		str = charjoin(str, SPACE2);
	else if (s[i] == '|')
		str = charjoin(str, PIPE);
	else if (s[i] == '>')
		str = charjoin(str, REDR);
	else if (s[i] == '<')
		str = charjoin(str, REDL);
	else
		str = charjoin(str, s[i]);
	return (str);
}

char	*remove_space(char *s, char c, int *i)
{
	char	*str;

	str = ft_strdup("");
	if ((*i) >= ft_strlen(s))
		return (s);
	while (s[(*i)])
	{
		if (s[(*i)] == c)
		{
			(*i)++;
			if (s[(*i)] == c)
				str = charjoin(str, NL);
			while (s[(*i)] != c && s[(*i)])
			{
				str = remove_space_utils(s, c, str, *i);
				(*i)++;
			}
		}
		else
			str = charjoin(str, s[(*i)]);
		(*i)++;
	}
	free(s);
	return (str);
}