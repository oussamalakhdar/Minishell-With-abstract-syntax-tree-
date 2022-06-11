/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/11 12:33:37 by olakhdar         ###   ########.fr       */
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
			line = putspace(line);
			printf("~~ %s\n", line);
			str = ft_split(line, ' ');
			while(str[i])
				printf("-- %s\n", str[i++]);
		}
	}
	return 0;
}
