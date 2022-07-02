/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 18:23:49 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 14:14:40 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*pt;
	char	*ss1;
	char	*ss2;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ss1 = (char *)s1;
	ss2 = (char *)s2;
	if (!ss1 || !ss2)
		return (NULL);
	pt = malloc(sizeof(char ) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (pt == NULL)
		return (NULL);
	while (ss1[i])
	{
		pt[i] = ss1[i];
		i++;
	}
	while (ss2[j])
		pt[i++] = ss2[j++];
	pt[i] = '\0';
	free(s1);
	return (pt);
}

void	close_read_f(int fd, char *str)
{
	if (!str)
		write(1, "\n", 1);
	if (close(fd) == -1)
	{
		perror("close fail\n");
		exit(1);
	}
}

void	putinfile(int fd, char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int	myfork(void)
{
	int	pid;

	pid = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork fail");
	}
	return (pid);
}

int	ft_strcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	if (!s1)
		return (-1);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - s2[i]);
		i++;
	}
	return (0);
}
