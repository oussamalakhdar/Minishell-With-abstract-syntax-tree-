/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/17 15:04:39 by abayar           ###   ########.fr       */
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
	int		fd;
	char	*str;
	
	j = 0;
	fd = 0;
	str = NULL;
	while (s[j])
	{
		if (s[j][0] == c && s[j][1] != c && s[j + 1][0] != c)
		{
			str = ft_strdup("");
			// j++;
			// if (c == '<')
			// 	fd = open(s[j], O_RDONLY);
			// else
			// 	fd = open(s[j], O_CREAT | O_WRONLY | O_TRUNC, 777);
			free(str);
			str = NULL;
			//printf("%d\n", tmp);
			str = ft_strdup(s[++j]);
			//printf("%s\n", str);
		}
		// if (c == '>')
		// {
		// 	if (s[j][0] == '>' && s[j][1] == '>' && s[j + 1][0] != '>')
		// 	{
		// 		fd = open(s[++j], O_CREAT | O_WRONLY | O_APPEND, 777);
		// 		if (fd < 0)
		// 			write(2, "open fail\n", 10);
		// 	}
		// }
		// tmp = fd;
		j++;
	}
	// while (--tmp >= 3)
	// 	close(tmp);
	return (str);
}

cmd	*redirect_cmd(cmd *exec, char **s, int *i)
{
	redir	*cmdd;
	execcmd	*excmd;

	excmd = (execcmd *)exec;
	cmdd = malloc(sizeof(*cmdd));
	cmdd->type = '>';
	if (excmd->infile)
		cmdd->infd = open(excmd->infile, O_RDONLY);
	else
		cmdd->infd = -2;
	if (excmd->outfile)
		cmdd->outfd = open(excmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		cmdd->outfd = -2;
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
	// if (cmdd->outfd == 0)
	// 	cmdd->outfd = 1;
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

void	runcmd(cmd *cmdd, int *p, int *c)
{
	ppipe		*pcmd;
	execcmd		*execcmdd;
	redir		*rcmd;
	int			i;
	char		*str;
	char		*temp;
	int			pp[2];

	i = 0;
	if (cmdd->type == '|')
	{
		///printf("****  PIPE  ******\n");
		(*c)++;
		pcmd = (ppipe *)cmdd;
		pipe(pp);
		int pid = fork();
		if (pid == 0)
		{
			close(pp[0]);
			dup2(pp[1], STDOUT_FILENO);
			//printf("****   ******\n");
			runcmd(pcmd->left, p, c);
			//close(pp[1]);
		}
		// else
		// {
		(*p)++;
		wait(0);
		close(pp[1]);
		dup2(pp[0], STDIN_FILENO);
		runcmd(pcmd->right, p, c);
		close(pp[0]);
		//}
	}
	else if (cmdd->type == ' ')
	{
		//printf("****  EXEC  ******\n");
		execcmdd = (execcmd *)cmdd;
		// int id = fork();
		// if (id == 0)
		// {
			// dup2(execcmdd->infd, STDIN_FILENO);
			// dup2(execcmdd->outfd, STDOUT_FILENO);
			while (execcmdd->path[i])
			{
				str = ft_strjoin(execcmdd->path[i], "/");
				temp = str;
				str = ft_strjoin(str, execcmdd->argv[0]);
				if (access(str, F_OK) != -1)
					execve(str, execcmdd->argv, NULL);
				free(str);
				i++;
			}
		// }
		//wait(0);
	}
	else if (cmdd->type == '>')
	{
		//printf("****  REDERIC  ******\n");
		rcmd = (redir *)cmdd;
		execcmdd = (execcmd *)rcmd->cmdn;
		//printf("**  infd -> %d    ****outfd -> %d****\n", rcmd->infd, rcmd->outfd);
		if (*c == 0)
		{
			int id = myfork();
			if (id == 0)
			{
				if (rcmd->infd != -2)
					dup2(rcmd->infd, STDIN_FILENO);
				if (rcmd->outfd != -2)
					dup2(rcmd->outfd, STDOUT_FILENO);
				runcmd(rcmd->cmdn, p, c);
			}
			wait(0);
			if (rcmd->outfd)
				close(rcmd->outfd);
			if (rcmd->infd)
				close(rcmd->infd);
		}
		else
		{
			runcmd(rcmd->cmdn, p, c);	
		}
		// if (*p == 0)
		// {
		// 	printf("**  infd -> %d    ****outfd -> %d****\n", rcmd->infd, rcmd->outfd);
		// 	if (rcmd->infd != -1)
		// 		dup2(rcmd->infd, STDIN_FILENO);
		// 	// else
		// 	// 	dup2(0, STDIN_FILENO);
		// 	if (rcmd->outfd != -1)
		// 		dup2(rcmd->outfd, STDOUT_FILENO);
		// 	// else if (c == 0)
		// 	// 	dup2(1, STDOUT_FILENO);
		// }	
		// else
		// {
		// 	if (rcmd->outfd != -1)
		// 		dup2(rcmd->outfd, STDIN_FILENO);
		// 	else
		// 		dup2(0, STDIN_FILENO);
		// }
		//runcmd(rcmd->cmdn, p, c);
		// }
		// if (rcmd->outfd)
		// 	close(rcmd->outfd);
		// if (rcmd->infd)
		// 	close(rcmd->infd);
		
		// wait(0);
		// if (execcmdd->infile)
		// 	rcmd->infd = open(execcmdd->infile, O_RDONLY);
		// else
		// 	rcmd->infd = 0;
		// if (execcmdd->outfile)
		// 	rcmd->outfd = open(execcmdd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 755);
		// else
		// 	rcmd->outfd = 1;
		// dup2(0, STDOUT_FILENO);
		// dup2(1, STDIN_FILENO);
		// close(rcmd->infd);
		// close(rcmd->outfd);
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
			//printf("line == %s",line);
			if (!line || ft_strcmp(line, "exit") == 0)
				return 0;
			if (ft_strncmp(line, "\n", ft_strlen(line)))
				continue;
			add_history(line);
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
			int	p = 0,c = 0;
			runcmd(magic_time(str, &i, envp), &p, &c);
			// free(line);
			// line = NULL;
			// free(str);
		}
	}
	return 0;
}
