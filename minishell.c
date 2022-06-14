/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/14 13:07:36 by olakhdar         ###   ########.fr       */
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

int	chrr(char **s, int *i)
{
	while (s[*i])
	{
		if (s[*i][0] == '|')//&& s[*i][0] == '\n')
			return (1);
		(*i)++;
	}
	return (0);
}

int	tablen(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	**parceline(char **s, int *i)
{
	char	**n;
	int		j;
	int		tmp;

	j = 0;
	tmp = (*i);
	if (s[*i] == NULL || tablen(s) <= (*i))
		return (NULL);
	//chrr(s, i);
	while (s[*i])
	{
		if (s[*i][0] == '|')
			break ;
		(*i)++;
	}
	n = malloc(sizeof(char *) * ((*i) - tmp) + 1);
	int k = (*i) - tmp;
	while (j < k)
	{
		n[j] = ft_strdup(s[tmp]);
		tmp++;
		j++;
	}
	n[j] = 0;
	(*i)++;
	return (n);
}

cmd	*pipecmd(cmd *left, cmd *right)
{
	ppipe	*cmdd;

	cmdd = malloc(sizeof(*cmdd));
	cmdd->type = '|';
	cmdd->left = left;
	cmdd->right = right;
	return ((cmd*) cmdd);
}


cmd	*execnode(char **s)
{
	execcmd	*cmdd;
	
	cmdd = malloc(sizeof(*cmdd));
	cmdd->type = ' ';
	cmdd->argv = s;
	return ((cmd*) cmdd);
}

cmd	*parce_redir(char **s)
{
	cmd		*cmdd;
	redir	*rcmd;

	cmdd = (cmd *)execnode(s);
	//--------check
	return ((cmd *)cmdd);
}

cmd *parce_pipe(char **str, int *i)
{
	cmd		*cmdd;
	//cmd		*pcmd;

	printf("**********\n");
	cmdd = parce_redir(parceline(str, i));
	if (str[(*i) - 1][0] == '|')
	{
		cmdd = pipecmd(cmdd, parce_pipe(str, i));
	}
	//printf("%s\n", str[(*i) - 1]);
	return ((cmd *)cmdd);
}


cmd	*magic_time(char **s, int *i)
{
	cmd			*cmdd;
	cmd		*pcmd;

	cmdd	= parce_pipe(s, i);
	// if (chrr(s, 0) == 0)
	// 	pcmd = pipecmd(cmdd, magic_time(s, i));
	return ((cmd*) pcmd);
}

#include <string.h>
int main(int argc, char **argv,char **envp)
{
	char 			*line;
	char 			**str;
	cmd				*cmd;
	(void)argv;
	

	int i = 0;
	char **S = NULL;
	if (argc == 1)
	{
		while(1)
		{
			i = 0;
			line =  readline("𝖒𝖎𝖓𝖎𝖘𝖍𝖊𝖑𝖑➜ ");
			add_history(line);
			if (line == NULL)
				return 0;
			if (ft_strncmp(line, "env", ft_strlen(line)))
				printenv(envp);
			if (line[0] == 'c' && line[1] == 'd' && line[2] == ' ')
			{
				//execve("/bin/cat", S, NULL);
				chdir(line + 3);
			}
			line = putspace(line);
			str = ft_split(line, ' ');
			undo(str);
			int j=0,p=0;
			magic_time(str, &i);
			// while ((S = parceline(str, &i)) > 0)
			// {
			// 	j = 0;
			// 	while (S[j])
			// 	{
			// 		printf("%s\n",S[j]);
			// 		//printf("%d\n",j);
			// 		j++;
			// 	}
			// 	//printf("_______________%d______________\n",p++);
			// }
			
			//cmd = magic_time(str, &i);
			//parceline(str, &i);
			//int	f=parceline(str, &i);
			
		}
	}
	// system("leaks minishell");
	return 0;
}
