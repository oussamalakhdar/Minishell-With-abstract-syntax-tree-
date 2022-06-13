/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/13 02:53:49 by abayar           ###   ########.fr       */
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
	while (s[i])
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

int	chrr(char **s, int *i)
{
	while (s[*i])
	{
		if (s[*i][0] == '|')
			return (1);
		(*i)++;
	}
	return (0);
}

char	**parceline(char **s, int *i)
{
	char	**n;
	int		j;
	int		tmp;

	j = 0;
	tmp = (*i);
	chrr(s, i);
	n = malloc(sizeof(char *) * ((*i) - tmp) + 1);
	while (j <= (*i) - tmp)
	{
		n[j] = ft_strdup(s[tmp]);
		tmp++;
		j++;
	}
	n[j] = 0;
	(*i)++;
	return (n);
}

struct cmd	*pipecmd(struct cmd *left, struct cmd *right)
{
	ppipe	*cmd;

	cmd = malloc(sizeof(*cmd));
	cmd->type = '|';
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd*) cmd);
}


struct	cmd	*execnode(char **s)
{
	execcmd	*cmd;
	
	cmd = malloc(sizeof(*cmd));
	cmd->type = ' ';
	cmd->argv = s;
	return ((struct cmd*) cmd);
}

struct cmd	*magic_time(char **s, int *i)
{
	struct cmd		*cmd;
	struct cmd		*pcmd;

	cmd	= execnode(parceline(s, i));
	if (chrr(s, 0) == 0)
		return ((struct cmd*) cmd);
	else
		pcmd = pipecmd(cmd, magic_time(s, i));
	return ((struct cmd*) pcmd);
}

int main(int argc, char **argv,char **envp)
{
	char 			*line;
	char 			**str;
	struct cmd				*cmd;
	(void)argv;
	

	int i = 0;
	char **S;
	if (argc == 1)
	{
		while(1)
		{
			i = 0;
			line =  readline("𝖒𝖎𝖓𝖎𝖘𝖍𝖊𝖑𝖑➜ ");
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
			//cmd = magic_time(str, &i);
			//S = parceline(str, &i);
			//int	f=parceline(str, &i);
		}
	}
	return 0;
}
