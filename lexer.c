/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 21:37:16 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 14:52:53 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
