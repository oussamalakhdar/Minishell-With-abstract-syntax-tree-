/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 15:44:24 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/09 12:18:53 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strndup(char const *s1, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = malloc((n + 1) * (sizeof(char)));
	if (str == NULL)
		return (NULL);
	while (s1[i] && i < n)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static int	ft_len(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

static int	ft_cmpt(char const *s, char c)
{
	int	i;
	int	cmpt;

	i = 0;
	cmpt = 0;
	while (s[i] == c)
		i++;
	while (s[i])
	{
		if (s[i] == c && s[i + 1] == '\0')
			break ;
		if (s[i] == c && s[i + 1] != c)
			cmpt++;
		i++;
	}
	return (cmpt + 1);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**s1;

	i = 0;
	j = 0;
	if (s == NULL)
		return (NULL);
	s1 = malloc(sizeof(char *) * (1 + ft_cmpt(s, c)));
	if (s1 == NULL)
		return (NULL);
	while (s[i])
	{
		if (s[i] != c)
		{
			s1[j] = ft_strndup(s + i, ft_len(s + i, c));
			i = i + ft_len(s + i, c);
			j++;
		}
		else if (s[i] == c)
			i++;
	}
	s1[j] = NULL;
	return (s1);
}
