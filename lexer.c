/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 21:37:16 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 12:22:23 by abayar           ###   ########.fr       */
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

int	add_value_utils7(char **ret, char *s, int *i, t_env **env)
{
	char	*name;
	char	*value;

	name = NULL;
	add_value_utils2(s, (*i));
	(*i) = add_value_utils3(s, (*i), ret);
	if (s[(*i)] == '$')
	{
		if (!add_value_utils4(&name, ret, s, (*i)))
			return (1);
		name = ft_strdup(find_dollar(&s[(*i)]));
		add_value_utils5(name);
		value = scan_list(name, env);
		(*i)++;
		add_value_utils6(value, ret, s, (*i));
		while (s[(*i)] && !is_specialchar(s[(*i)]))
			(*i)++;
		(*i)--;
		if (name)
			free(name);
	}
	else
		*ret = charjoin(*ret, s[(*i)]);
	return (0);
}

char	*add_value(char *s, t_env **env)
{
	char	*ret;
	int		i;

	i = 0;
	if (!find_dollar(s))
		return (s);
	ret = ft_strdup("");
	while (s[i])
	{
		if (add_value_utils7(&ret, s, &i, env))
			break ;
		i++;
	}
	if (s)
		free(s);
	return (ret);
}
