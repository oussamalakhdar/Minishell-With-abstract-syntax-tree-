/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 21:37:16 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/01 15:48:46 by abayar           ###   ########.fr       */
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

void	add_value_utils3(char *s, int *i, char *ret)
{
	if (s[(*i)] == '\'' \
		|| ((*i) > 1 && s[(*i) - 1] == '<' && s[(*i) - 2] == '<'))
	{
		if (s[(*i) - 1] != '<')
		{
			ret = charjoin(ret, s[(*i++)]);
			while (s[(*i)] != '\'' && s[(*i)])
			{
				if (s[(*i)] == '\"')
					ret = charjoin(ret, QUOT2);
				else
					ret = charjoin(ret, s[(*i)]);
				(*i)++;
			}
		}
		else
		{
			while (s[(*i)] && (s[(*i)] == SPACE2 || s[(*i)] == ' '))
				(*i)++;
			while (s[(*i)] && s[(*i)] != SPACE2 && s[(*i)] != ' ')
				ret = charjoin(ret, s[(*i++)]);
		}
	}
}

int	add_value_utils4(char *name, char **ret, char *s, int i)
{
	if (s[(i) + 1] == '?')
	{
		name = ft_strdup(s);
		name[(i)] = '\0';
		*ret = ft_strjoin(name, ft_itoa(g_status));
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

void	add_value_utils6(char *value, char **ret, char *s, int i)
{
	int	p;

	p = 0;
	if (value != NULL)
	{
		while (value[p])
		{
			*ret = charjoin(*ret, value[p]);
			p++;
		}
	}
	else if (is_specialchar(s[i]))
		*ret = charjoin(*ret, '$');
	else
		*ret = charjoin(*ret, NL);
}

char	*add_value(char *s, t_env **env)
{
	char	*ret;
	char	*name;
	char	*value;
	int		i;
	int		p;

	i = 0;
	name = NULL;
	if (!find_dollar(s))
		return (s);
	ret = ft_strdup("");
	while (s[i])
	{
		add_value_utils2(s, i);
		add_value_utils3(s, &i, ret);
		if (s[i] == '$')
		{
			if (!add_value_utils4(name, &ret, s, i))
				break ;
			name = ft_strdup(find_dollar(&s[i]));
			add_value_utils5(name);
			value = scan_list(name, env);
			i++;
			p = 0;
			add_value_utils6(value, &ret, s, i);
			while (s[i] && !is_specialchar(s[i]))
				i++;
			i--;
			if (name)
				free(name);
		}
		else
			ret = charjoin(ret, s[i]);
		i++;
	}
	if (s)
		free(s);
	return (ret);
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
		i++;
	}
	free(s);
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
				if (c == '\"' && s[(*i)] == '\'')
					str = charjoin(str, QUOT);
				if (c == '\'' && s[(*i)] == '\"')
					str = charjoin(str, QUOT2);
				if (s[(*i)] == ' ')
					str = charjoin(str, SPACE2);
				else if (s[(*i)] == '|')
					str = charjoin(str, PIPE);
				else if (s[(*i)] == '>')
					str = charjoin(str, REDR);
				else if (s[(*i)] == '<')
					str = charjoin(str, REDL);
				else
					str = charjoin(str, s[(*i)]);
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
