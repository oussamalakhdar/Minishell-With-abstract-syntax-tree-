/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/14 20:55:02 by abayar           ###   ########.fr       */
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

int	checker(char **s, char c, int *i)
{
	int j;

	j = (*i);
	if (tablen(s) <= (*i))
		return 0;
	while (s[j])
	{
		if (s[j][0] == c)
			return 1;
		j++;
	}
	return 0;
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

char *getfiles(char **s, char c, int *i)
{
	int		j;
	int		k;
	int		count;
	char	*str;
	
	j = (*i);
	k = 0;
	count = 0;
	if (tablen(s) <= (*i))
		return 0;
	// while(s[j])
	// {
	// 	if (s[j][0] == c && s[j + 1][0] != c)
	// 		count++;
	// 	j++;
	// }
	//str = malloc(count * sizeof(char*) + 1);
	j = (*i);
	while (s[j])
	{
		if (s[j][0] == c && s[j][1] != c && s[j + 1][0] != c)
		{
			free(str);
			str = ft_strdup(s[++j]);
		}
		j++;
	}
	//str[k] = NULL;
	return (str);
}

cmd	*redirect_cmd(cmd *exec, char **s, int *i)//, char *file)
{
	redir	*cmdd;
	execcmd	*excmd;

	excmd = (execcmd *)exec;
	cmdd = malloc(sizeof(*cmdd));
	cmdd->type = '>';
	if (!excmd->infile)
		cmdd->infd = open(excmd->infile, O_RDONLY);
	else
		cmdd->infd = 0;
	if (!excmd->outfile)
		cmdd->infd = open(excmd->outfile, O_CREAT | O_WRONLY | O_TRUNC);
	else
		cmdd->outfd = 1;
	cmdd->cmdn = exec;
	return ((cmd*) cmdd);
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


cmd	*execnode(char **s, int *i)
{
	execcmd	*cmdd;
	
	cmdd = malloc(sizeof(*cmdd));
	cmdd->type = ' ';
	cmdd->argv = s;//------------------------------------------------------------
	cmdd->infile = getfiles(s, '<', i);
	cmdd->outfile = getfiles(s, '>', i);
	cmdd = (execcmd*)redirect_cmd((cmd *)cmdd, s, i);
	// if (cmdd->infile != NULL)// || cmdd->outfile != NULL)
	// 	return ((cmd*)redirect_cmd(cmdd, s, i, '<'));
	// if (cmdd->outfile != NULL)// || cmdd->outfile != NULL)
	// 	return ((cmd*)redirect_cmd(cmdd, s, i, '>'));
	return ((cmd*) cmdd);
}

// cmd	*parce_redir(char **s)
// {
// 	cmd		*cmdd;
// 	redir	*rcmd;

// 	cmdd = execnode(s);
// 	//--------check
// 	return (cmdd);
// }


cmd *parce_pipe(char **str, int *i)
{
	cmd		*cmdd;
	//cmd		*pcmd;

	if (checker(str, '|', i))
	{
		cmdd = pipecmd(execnode(parceline(str, i), i), parce_pipe(str, i));
	}
	else
		cmdd = execnode(parceline(str, i), i);
	// else
	// 	printf("hiyahadii\n");
	//printf("%s\n", str[(*i) - 1]);
	return (cmdd);
}


cmd	*magic_time(char **s, int *i)
{
	cmd			*cmdd;
	cmd		*pcmd;

	cmdd	= parce_pipe(s, i);
	// if (chrr(s, 0) == 0)
	// 	pcmd = pipecmd(cmdd, magic_time(s, i));
	return ((cmd*) cmdd);
}

void	runcmd(cmd *cmdd)
{
	ppipe		*pcmd;
	execcmd		*execcmdd;
	redir		*rcmd;

	if (cmdd->type == '|')
	{
		printf("**********\n");
		pcmd = (ppipe *)cmdd;
		int pid = fork();
		if (pid == 0)
		{
			if (pcmd->left->type == ' ')
			{
				execcmdd = (execcmd *)pcmd->left;
				printf("lisser ------> awel string %s\n", execcmdd->argv[0]);
				exit(0);
			}
			// else if (pcmd->left->type == '>')
			// {
			// 	execcmdd = (execcmd *)pcmd->right;
			// 	printf("liMEN ------> awel string %s\n", execcmdd->argv[0]);
			// }
		}
		wait(0);
		if (pcmd->right->type == '|')
			runcmd(pcmd->right);
		else if (pcmd->right->type == ' ')
		{
			execcmdd = (execcmd *)pcmd->right;
			printf("liMEN ------> awel string %s\n", execcmdd->argv[0]);
		}
		else if (pcmd->right->type == '>')
		{
			execcmdd = (execcmd *)pcmd->right;
			printf("liMEN ------> awel string %s\n", execcmdd->argv[0]);
		}
	}
	else if (cmdd->type == ' ')
	{
		printf("type ---> %c\n", cmdd->type);
		execcmdd = (execcmd *)cmdd;
		printf("--<%c>\n", execcmdd->type);
		printf("--%s\n--%s", execcmdd->argv[0], execcmdd->argv[1]);
		//printf("***************\n");
	}
	else if (cmdd->type == '>')
	{
		rcmd = (redir *)cmdd;
		//printf("---------hadi redir -----> o hadi l type d exec node  \"%c\"\n", rcmd->cmdn->type);
	}
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
			if (line == NULL || ft_strncmp(line, "exit", ft_strlen(line)))
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
			// S = getfiles(str, '>', &i);
			// while(S[j])
			// 	printf("--%s\n",S[j++]);
		//	magic_time(str, &i);
			runcmd(magic_time(str, &i));
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
