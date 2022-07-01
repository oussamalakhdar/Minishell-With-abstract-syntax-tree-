/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/01 21:36:59 by abayar           ###   ########.fr       */
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

void	runcmdexec(t_cmd *cmdd, t_env **env)
{
	int			exstatus;
	t_execcmd	*execcmdd;
	int			i;
	char		*str;
	char		*temp;
	
	i = 0;
	execcmdd = (t_execcmd *)cmdd;
	builtins(execcmdd->argv, env);
	if (execcmdd->path == NULL)
	{
		perror("Path not found");
		exit(1);
	}
	if (access(execcmdd->argv[0], F_OK) != -1)
		execve(execcmdd->argv[0], execcmdd->argv, NULL);
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
	if (execcmdd->argv[0][0] == '.' && execcmdd->argv[0][1] == '/')
	{
		if (access(execcmdd->argv[0] + 2, F_OK) == -1)
			perror("Path Not Found!");
	}
	else
		perror("Path Not Found!");
	g_status = WEXITSTATUS(exstatus);
	exit(g_status);
}

void	runcmdpipe(t_cmd *cmdd, int *c, t_env **env, t_env **exportt)
{
	t_ppipe		*pcmd;
	int			pp[2];
	t_redir		*rcmd;
	int			exstatus;
	
	(*c)++;
	pcmd = (t_ppipe *)cmdd;
	pipe(pp);
	int	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, handlle);
		close(pp[0]);
		dup2(pp[1], STDOUT_FILENO);
		runcmd(pcmd->left, env, exportt, c);
		close(pp[1]);
		if (*c >= 1)
			exit(0);
	}
	else
	{
		if (pcmd->right->type == '>')
		{
			rcmd = (t_redir *)pcmd->right;
			*c = -1;
		}
		close(pp[1]);
		dup2(pp[0], STDIN_FILENO);
		runcmd(pcmd->right, env, exportt, c);
		close(pp[0]);
		close(pp[1]);
		waitpid(pid, &exstatus, 0);
		g_status = WEXITSTATUS(exstatus);
		dup2(1, STDIN_FILENO);
	}
}

void	runcmd(t_cmd *cmdd, t_env **env, t_env **exportt, int *c)
{
	t_execcmd	*execcmdd;
	t_redir		*rcmd;
	int			j;
	int			exstatus;

	j = 1;
	if (cmdd->type == '|')
		runcmdpipe(cmdd, c, env, exportt);
	else if (cmdd->type == ' ')
		runcmdexec(cmdd, env);
	else if (cmdd->type == '>')
	{
		rcmd = (t_redir *)cmdd;
		execcmdd = (t_execcmd *)rcmd->cmdn;
		if (rcmd->outfd != -2 && rcmd->app == 0)
			open(execcmdd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (*c == 0 || *c == 1)
		{
			if (check_cmd(execcmdd->argv[0]) == 'u')
			{
				j = 1;
				while (execcmdd->argv[j])
				{
					unset(execcmdd->argv[j], env);
					unset(execcmdd->argv[j], exportt);
					j++;
				}
			}
			else if (check_cmd(execcmdd->argv[0]) == 'x')
				export(execcmdd->argv, env, exportt);
			else
			{
				int	id = myfork();
				if (id == 0)
				{
					signal(SIGQUIT, handlle);
					if (rcmd->infd == -1)
						exit(1);
					if (rcmd->infd != -2)
						dup2(rcmd->infd, STDIN_FILENO);
					if (rcmd->outfd != -2)
						dup2(rcmd->outfd, STDOUT_FILENO);
					runcmd(rcmd->cmdn, env, exportt, c);
				}
				waitpid(id, &exstatus, 0);
				g_status = WEXITSTATUS(exstatus);
				if (*c == 0)
				{
					if (rcmd->outfd)
						close(rcmd->outfd);
					if (rcmd->infd)
						close(rcmd->infd);
				}
			}
		}
		else if (*c == -1)
		{
			int	id = myfork();
			if (id == 0)
			{
				signal(SIGQUIT, handlle);
				if (rcmd->infd != -2)
					dup2(rcmd->infd, STDIN_FILENO);
				if (rcmd->outfd != -2)
					dup2(rcmd->outfd, STDOUT_FILENO);
				if (check_cmd(execcmdd->argv[0]) == 'u')
				{
					j = 1;
					while (execcmdd->argv[j])
					{
						unset(execcmdd->argv[j], env);
						unset(execcmdd->argv[j], exportt);
						j++;
					}
				}
				else if (check_cmd(execcmdd->argv[0]) == 'x')
					export(execcmdd->argv, env, exportt);
				else
					runcmd(rcmd->cmdn, env, exportt, c);
			}
			else
			{
				waitpid(id, &exstatus, 0);
				g_status = WEXITSTATUS(exstatus);
				if (rcmd->outfd)
					close(rcmd->outfd);
				if (rcmd->infd)
					close(rcmd->infd);
			}
		}
		if (*c > 1)
		{
			if (rcmd->infd != -2)
				dup2(rcmd->infd, STDIN_FILENO);
			if (rcmd->outfd != -2)
				dup2(rcmd->outfd, STDOUT_FILENO);
			if (check_cmd(execcmdd->argv[0]) == 'u')
			{
				j = 1;
				while (execcmdd->argv[j])
				{
					unset(execcmdd->argv[j], env);
					unset(execcmdd->argv[j], exportt);
					j++;
				}
			}
			else if (check_cmd(execcmdd->argv[0]) == 'x')
				export(execcmdd->argv, env, exportt);
			else
				runcmd(rcmd->cmdn, env, exportt, c);
		}
	}
}

void	free_tree(t_cmd *tree, int	*r)
{
	t_ppipe		*pipenode;
	t_execcmd	*execnode;
	t_redir		*redirnode;

	if (tree->type == '|')
	{
		(*r)++;
		pipenode = (t_ppipe *)tree;
		free_tree(pipenode->left, r);
		free_tree(pipenode->right, r);
		free(pipenode);
	}
	else if (tree->type == '>')
	{
		redirnode = (t_redir *)tree;
		free_tree(redirnode->cmdn, r);
		free(redirnode);
	}
	else if (tree->type == ' ')
	{
		execnode = (t_execcmd *)tree;
		free_all(execnode->path);
		if (execnode->argv != NULL)
			free_all(execnode->argv);
		if (execnode->infile)
			free(execnode->infile);
		if (execnode->outfile)
			free(execnode->outfile);
		free(execnode);
	}
}

void	flip_free(t_cmd	*cmd)
{
	t_ppipe	*pcmd;

	if (!cmd)
		return ;
	if (cmd->type == '|')
	{
		pcmd = (t_ppipe *)cmd;
		if (pcmd->right != NULL)
			flip_free(pcmd->right);
		free(cmd);
	}
}


void	minishell(char **str, char **envp, t_ex *data, char *line)
{
	t_cmd	*cmd;
	int		i;
	int		c;

	i = 0;
	c = 0;
	cmd = magic_time(str, &i, envp, &data->new);
	runcmd(cmd, &data->new, &data->new2, &c);
	free(line);
	line = NULL;
	free_all(str);
	c = 0;
	free_tree(cmd, &c);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**str;
	t_env	*env;
	t_env	*exportt;
	t_ex	data;

	(void) argv;
	env = NULL;
	exportt = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handlle);
	if (argc == 1)
	{
		createnv(&env, envp);
		createnv(&exportt, envp);
		data.new = env;
		data.new2 = exportt;
		while (1)
		{
			line = readline("𝖒𝖎𝖓𝖎𝖘𝖍𝖊𝖑𝖑➜ ");
			signal(SIGINT, handlle);
			if (!line)
				return (0);
			if (find_space(line))
				continue ;
			if (ft_strncmp(line, "\n", ft_strlen(line)))
			{
				free(line);
				line = NULL;
				continue ;
			}
			add_history(line);
			if (line[0] == 'c' && line[1] == 'd' && line[2] == ' ')
			{
				if (chdir(line + 3) < 0)
					perror("Error");
				free(line);
				line = NULL;
				continue ;
			}
			line = putspace(line, &env);
			if (!line)
				continue ;
			str = ft_split(line, ' ');
			undo(str);
			if (!checkerrors(str))
			{
				free(line);
				line = NULL;
				free_all(str);
				continue ;
			}
			minishell(str, envp, &data, line);
		}
	}
	return (0);
}
