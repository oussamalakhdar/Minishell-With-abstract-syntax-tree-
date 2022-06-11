/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/11 15:41:12 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	printenv(char **envp)
{
	t_env *env;
	t_env *temp;

	env = NULL;
	createnv(&env, envp);
	temp = env;
	while(temp)
	{
		printf("%s=%s\n", temp->var_name, temp->var_value);		
		temp = temp->next;
	}
}

void	undo(char **s)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i][j])
	{
		if (s[i][0] == '\"')
		{
			j = 0;
			while (s[i][j])
			{
				if (s[i][j] == PIPE)
					s[i][j] = '|';
				if (s[i][j] == SPACE)
					s[i][j] = ' ';
				if (s[i][j] == REDR)
					s[i][j] = '>';
				if (s[i][j] == REDL)
					s[i][j] = '<';
				j++;
			}
		}
		i++;
	}
}

int main(int argc, char **argv,char **envp)
{
	char *line;
	char **str;
	(void)argv;

	int i = 0;
	if (argc == 1)
	{
		while(1)
		{
			line =  readline("𝖒𝖎𝖓𝖎𝖘𝖍𝖊𝖑𝖑➜ ");
			if (line == NULL)
				return 0;
			if (ft_strncmp(line, "env", ft_strlen(line)))
				printenv(envp);
			if (line[0] == 'c' && line[1] == 'd' && line[2] == ' ')
				chdir(line + 3);
			line = putspace(line);
			//printf("~~ %s\n", line);
			str = ft_split(line, ' ');
			undo(str);
			// while(str[i])
			// 	printf("-- %s\n", str[i++]);
		}
	}
	return 0;
}
