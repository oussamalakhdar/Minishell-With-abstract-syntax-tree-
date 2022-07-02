/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 14:05:37 by abayar           ###   ########.fr       */
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

void	run_exec_utils(t_execcmd *execcmdd)
{
	if (execcmdd->argv[0][0] == '.' && execcmdd->argv[0][1] == '/')
	{
		if (access(execcmdd->argv[0] + 2, F_OK) == -1)
			perror("Command Not Found!");
	}
	else
		perror("Command Not Found!");
}

void	run_run_utils(t_execcmd *execcmdd)
{
	if (execcmdd->path == NULL)
	{
		perror("Path not found");
		exit(1);
	}
	if (access(execcmdd->argv[0], F_OK) != -1)
		execve(execcmdd->argv[0], execcmdd->argv, NULL);
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
	run_run_utils(execcmdd);
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
	run_exec_utils(execcmdd);
	g_status = WEXITSTATUS(exstatus);
	exit(g_status);
}

void	run_left_pipe(t_ppipe *pcmd, int *c, t_ex *data, int *pp)
{
	signal(SIGQUIT, handlle);
	close(pp[0]);
	dup2(pp[1], STDOUT_FILENO);
	runcmd(pcmd->left, &data->new, &data->new2, c);
	close(pp[1]);
	if (*c >= 1)
		exit(0);
}

void	run_pp_utils(int pid, int *pp)
{
	int	exstatus;

	close(pp[0]);
	close(pp[1]);
	waitpid(pid, &exstatus, 0);
	g_status = WEXITSTATUS(exstatus);
	dup2(1, STDIN_FILENO);
}

int	pipefork(int *pp)
{
	int	pid;

	if (pipe(pp) < 0)
		perror("pipe failed");
	pid = fork();
	if (pid == -1)
		perror("fork failed");
	return (pid);
}

void	runcmdpipe(t_cmd *cmdd, int *c, t_env **env, t_env **exportt)
{
	t_ppipe		*pcmd;
	int			pp[2];
	t_redir		*rcmd;
	t_ex		data;
	int			pid;

	(*c)++;
	data.new = *env;
	data.new2 = *exportt;
	pcmd = (t_ppipe *)cmdd;
	pid = pipefork(pp);
	if (pid == 0)
		run_left_pipe(pcmd, c, &data, pp);
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
		run_pp_utils(pid, pp);
	}
}

void	run_mid_cmd(t_redir *rcmd, int *c, t_env **env, t_env **exportt)
{
	t_execcmd	*execcmdd;
	int			j;

	execcmdd = (t_execcmd *)rcmd->cmdn;
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

void	run_lastcmd(t_env **env, t_env **exportt, t_redir *rcmd, int *c)
{
	t_execcmd	*execcmdd;
	int			j;

	execcmdd = (t_execcmd *)rcmd->cmdn;
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

void	run_firstone(t_env **env, t_env **exportt, t_redir *rcmd, int *c)
{
	int	id;
	int	exstatus;

	id = myfork();
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

void	run_cmd_first_utils(t_redir *rcmd, t_env **env, t_env **exportt, int *c)
{
	t_execcmd	*execcmdd;
	int			j;

	execcmdd = (t_execcmd *)rcmd->cmdn;
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
		run_firstone(env, exportt, rcmd, c);
}

void	run_motherfucker_run(t_redir *rcmd, t_env **env, t_env **rtt, int *c)
{
	int	id;
	int	exstatus;

	id = myfork();
	if (id == 0)
		run_lastcmd(env, rtt, rcmd, c);
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

void	runcmd(t_cmd *cmdd, t_env **env, t_env **exportt, int *c)
{
	t_execcmd	*execcmdd;
	t_redir		*rcmd;

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
			run_cmd_first_utils(rcmd, env, exportt, c);
		else if (*c == -1)
			run_motherfucker_run(rcmd, env, exportt, c);
		if (*c > 1)
			run_mid_cmd(rcmd, c, env, exportt);
	}
}

void	free_tree_utils(t_cmd *tree)
{
	t_execcmd	*execnode;

	if (tree->type == ' ')
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

void	free_tree(t_cmd *tree, int	*r)
{
	t_ppipe		*pipenode;
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
	free_tree_utils(tree);
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

int	minishell_utils(char **line, t_env **env, char ***str)
{
	if (*line[0] == 'c' && *line[1] == 'd' && *line[2] == ' ')
	{
		if (chdir(*line + 3) < 0)
			perror("Error");
		free(*line);
		*line = NULL;
		return (0);
	}
	*line = putspace(*line, env);
	if (!*line)
		return (0);
	*str = ft_split(*line, ' ');
	undo(*str);
	if (!checkerrors(*str))
	{
		free(*line);
		*line = NULL;
		free_all(*str);
		return (0);
	}
	return (1);
}

int	minishell_utils2(char *line)
{
	if (!line)
		return (0);
	if (ft_strncmp(line, "\n", ft_strlen(line)))
	{
		free(line);
		line = NULL;
		return (1);
	}
	if (line && find_space(line))
	{
		free(line);
		line = NULL;
		return (1);
	}
	add_history(line);
	return (-1);
}

void	my_signal(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handlle);
}

int	micro_shell(t_ex *data, t_env **env, char **envp)
{
	char	*line;
	char	**str;

	str = NULL;
	while (1)
	{
		line = readline("𝖒𝖎𝖓𝖎𝖘𝖍𝖊𝖑𝖑➜ ");
		signal(SIGINT, handlle);
		if (minishell_utils2(line) == 1)
			continue ;
		if (minishell_utils2(line) == 0)
			return (0);
		if (!minishell_utils(&line, env, &str))
			continue ;
		minishell(str, envp, data, line);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	t_env	*exportt;
	t_ex	data;

	(void) argv;
	env = NULL;
	exportt = NULL;
	my_signal();
	if (argc == 1)
	{
		createnv(&env, envp);
		createnv(&exportt, envp);
		data.new = env;
		data.new2 = exportt;
		if (!micro_shell(&data, &env, envp))
			return (0);
	}
	return (0);
}
