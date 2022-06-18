/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 19:19:02 by abayar            #+#    #+#             */
/*   Updated: 2022/06/18 14:07:30 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../minishell.h"

char	*get_final_res(char **s)
{
	char	*temp;

	if (*s && **s != '\0')
	{
		temp = *s;
		*s = NULL;
		return (temp);
	}
	else
	{
		free(*s);
		*s = NULL;
		return (NULL);
	}
}

char	*get_result(char **s)
{
	char	*line;
	int		i;
	char	*temp;

	i = ft_strchr(*s, '\n');
	line = ft_substr(*s, 0, i + 1);
	temp = *s;
	*s = ft_substr(*s, i + 1, (ft_strlen(*s) - i));
	free(temp);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		buffer[BUFFER_SIZE + 1];
	int			ret;

	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	if (str == NULL)
		str = ft_strrdup("");
	while (ft_strchr(str, '\n') == -1)
	{
		ret = read(fd, buffer, BUFFER_SIZE);
		if (ret < 0)
		{
			free(str);
			str = NULL;
			return (NULL);
		}
		if (ret == 0)
			break ;
		buffer[ret] = '\0';
		str = ft_strjjoin(str, buffer);
	}
	if (ft_strchr(str, '\n') == -1)
		return (get_final_res(&str));
	return (get_result(&str));
}

// int main(){
// 	int fd = open("text.txt", O_RDWR);

// 	printf("%s",get_next_line(fd));
// 	printf("%s",get_next_line(fd));
// 	printf("%s",get_next_line(fd));
// 	printf("%s",get_next_line(fd));
// 	printf("%s",get_next_line(fd));
// 	system("leaks a.out");
// }
