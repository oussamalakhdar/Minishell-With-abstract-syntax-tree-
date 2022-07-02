/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:23:52 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 15:24:48 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	printenv(t_env **env, char *s)
{
	t_env	*temp;

	if (ft_strcmp(s, "env") == 0)
	{
		temp = *env;
		while (temp)
		{
			if (!temp->var_value)
				printf("%s=\n", temp->var_name);
			else if (ft_strcmp(temp->var_name, "PWD") != 0)
				printf("%s=%s\n", temp->var_name, temp->var_value);
			else
				printf("%s=%s\n", temp->var_name, pwd());
			temp = temp->next;
		}
	}
}

void	get_file_utils2(char **str, char **s, t_ex *data, int *j)
{
	int		fd;
	char	*name;

	fd = 0;
	if (*str)
		free(*str);
	*str = NULL;
	*str = ft_strdup(s[++(*j)]);
	if (data->c == '>')
		fd = open(*str, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
	{
		name = ft_strjoin(ft_strdup("/tmp/"), *str);
		fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0777);
		read_f(*str, fd, &data->new);
		free(*str);
		*str = name;
	}
	close(fd);
}

void	get_file_utils(char **str, char **s, char c, int *j)
{
	int	fd;

	fd = 0;
	if (*str)
		free(*str);
	*str = NULL;
	(*j)++;
	*str = ft_strdup(s[(*j)]);
	if (c == '<')
	{
		fd = open(*str, O_RDONLY);
		if (fd == -1)
			perror("Error ");
	}
	else
		fd = open(*str, O_CREAT | O_WRONLY, 0644);
	close(fd);
}

char	*getfiles(char **s, char c, t_env **env)
{
	int		j;
	int		fd;
	char	*str;
	t_ex	data;

	j = 0;
	fd = 0;
	data.new = *env;
	data.c = c;
	str = NULL;
	while (s[j])
	{
		if (s[j][0] == c && s[j][1] != c && s[j + 1][0] != c)
			get_file_utils(&str, s, c, &j);
		else if (s[j][0] == c && s[j][1] == c
				&& s[j][2] != c && s[j + 1][0] != c)
			get_file_utils2(&str, s, &data, &j);
		j++;
	}
	return (str);
}
