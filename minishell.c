/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/15 22:46:22 by abayar           ###   ########.fr       */
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

char	**get_path(char **env)
{
	int		i;
	char	**tmp;
	char	**str;

	i = 0;
	while (env[i])
	{
		if (env[i][0] == 'P' && env[i][2] == 'T')
		{
			str = ft_split(env[i], '=');
			tmp = str;
			str = ft_split(str[1], ':');
			free_all(tmp);
			return (str);
		}
		i++;
	}
	str = NULL;
	//perror("PATH not found\n");
	exit(1);
}

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

char	**scan_arg(char **s)
{
	int		j;
	int		count;
	char	**ss;

	j = 0;
	count = 0;
	while(s[j])
	{
		if ((s[j][0] == '>') || (s[j][0] == '<'))
			count++;
		j++;
	}
	ss = malloc(sizeof(char *) * (j - (count * 2) + 1));
	j = 0;
	count = 0;
	while (s[j])
	{
		if (s[j][0] == '<' || s[j][0] == '>')
		{
			j+=2;
			continue ;
		}
		else
			ss[count++] = s[j++];
	}
	ss[count] = 0;
	return (ss);
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

char *getfiles(char **s, char c)
{
	int		j;
	int		k;
	// int		count;
	char	*str;
	
	j = 0;
	k = 0;
	// count = 0;
	str = ft_strdup("");
	// if (tablen(s) <= (*i))
	// 	return 0;
	// while(s[j])
	// {
	// 	if (s[j][0] == c && s[j + 1][0] != c)
	// 		count++;
	// 	j++;
	// }
	//str = malloc(count * sizeof(char*) + 1);
	// j = (*i);
	while (s[j])
	{
		if (s[j][0] == c && s[j][1] != c && s[j + 1][0] != c)
		{
			free(str);
			str = ft_strdup(s[++j]);
			//printf("\nstr ==== %s\n--\n", str);
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
	if (excmd->infile)
		cmdd->infd = open(excmd->infile, O_RDONLY);
	else
		cmdd->infd = 0;
	if (excmd->outfile)
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


cmd	*execnode(char **s, int *i,char **env)
{
	execcmd	*cmdd;
	
	cmdd = malloc(sizeof(*cmdd));
	cmdd->type = ' ';
	cmdd->argv = scan_arg(s);//------------------------------------------------------------
	cmdd->infile = getfiles(s, '<');
	cmdd->outfile = getfiles(s, '>');
	cmdd->path = get_path(env);
	cmdd = (execcmd*)redirect_cmd((cmd *)cmdd, s, i);
	return ((cmd*) cmdd);
}

cmd *parce_pipe(char **str, int *i, char **env)
{
	cmd		*cmdd;
	//cmd		*pcmd;

	if (checker(str, '|', i))
	{
		cmdd = pipecmd(execnode(parceline(str, i), i, env), parce_pipe(str, i, env));
	}
	else
		cmdd = execnode(parceline(str, i), i, env);
	return (cmdd);
}


cmd	*magic_time(char **s, int *i, char **env)
{
	cmd			*cmdd;
	cmd		*pcmd;

	cmdd	= parce_pipe(s, i, env);
	// if (chrr(s, 0) == 0)
	// 	pcmd = pipecmd(cmdd, magic_time(s, i));
	return ((cmd*) cmdd);
}

void	runcmd(cmd *cmdd)
{
	ppipe		*pcmd;
	execcmd		*execcmdd;
	redir		*rcmd;
	int			i;
	char		*str;

	i = 0;
	if (cmdd->type == '|')
	{
		printf("****  PIPE  ******\n");
		pcmd = (ppipe *)cmdd;
		int pid = fork();
		if (pid == 0)
		{
			runcmd(pcmd->left);
		}
		wait(0);
		runcmd(pcmd->right);
	}
	else if (cmdd->type == ' ')
	{
		printf("****  EXEC  ******\n");
		execcmdd = (execcmd *)cmdd;
		int id = fork();
		if (id == 0)
		{
			while (execcmdd->path[i])
			{
				str = ft_strjoin(execcmdd->path[i], "/");
				str = ft_strjoin(str, execcmdd->argv[0]);
				if (access(str, F_OK) != -1)
					execve(str, execcmdd->argv, NULL);
				free(str);
				i++;
			}
		}
		wait(0);
	}
	else if (cmdd->type == '>')
	{
		printf("****  REDERIC  ******\n");
		rcmd = (redir *)cmdd;
		dup2(rcmd->infd, STDIN_FILENO);
		dup2(rcmd->outfd, STDOUT_FILENO);
		runcmd(rcmd->cmdn);
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
				chdir(line + 3);
				continue ;
			}
			line = putspace(line);
			str = ft_split(line, ' ');
			undo(str);
			runcmd(magic_time(str, &i, envp));
		}
	}
	// system("leaks minishell");
	return 0;
}
