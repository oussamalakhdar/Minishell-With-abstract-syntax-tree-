/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/30 14:14:34 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_degit(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] > '9' || s[i] < '0')
			return (0);
		i++;
	}
	return (1);
}

int	check_exit(char **s)
{
	if (ft_strcmp(s[0], "exit") == 0)
	{
		if (tablen(s) == 2)
		{
			if (is_degit(s[1]))
			{
				g_status = atoi(s[1]);
				exit(g_status);
			}
			else
			{
				perror("exit: numeric argument required");
				g_status = 1;
				exit(g_status);
			}
		}
		else if (tablen(s) > 2)
		{
			perror("exit: too many arguments");
			g_status = 1;
			return (0);
		}
		else
		{
			exit(0);
			return (0);
		}
	}
	return (1);
}

int	checkerrors(char **s)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!check_exit(s))
		return (0);
	while (s[i])
	{
		if ((s[i][0] == '>' || s[i][0] == '<') && (tablen(s) == (i + 1)))
		{
			perror("minishell: No file");
			return (0);
		}
		if (s[0][0] == '|' || s[tablen(s) - 1][0] == '|')
		{
			perror("minishell: syntax error");
			return (0);
		}
		if (s[i][0] == '<' && s[i + 1][0] == '>')
		{
			perror("minishell: syntax error");
			return (0);
		}
		if (s[i][0] == '>' && s[i + 1][0] == '<')
		{
			perror("minishell: error");
			return (0);
		}
		if ((s[i][0] == '<' && s[i][1] == '<' && s[i + 1][0] == '<')
			|| (s[i][0] == '<' && s[i][1] == '<' && s[i + 1][0] == '>'))
		{
			perror("minishell: error");
			return (0);
		}
		if ((s[i][0] == '>' && s[i][1] == '>' && s[i][2] == '>')
			|| (s[i][0] == '>' && s[i][1] == '>' && s[i + 1][0] == '<'))
		{
			perror("minishell: error");
			return (0);
		}
		if ((s[i][0] == '>' || s[i][0] == '<') && s[i + 1][0] == '|')
		{
			perror("minishell: error");
			return (0);
		}
		i++;
	}
	return (1);
}

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

char	**parceline(char **s, int *i)
{
	char	**n;
	int		j;
	int		tmp;
	int		k;

	j = 0;
	tmp = (*i);
	if (s[*i] == NULL || tablen(s) <= (*i))
		return (NULL);
	while (s[*i])
	{
		if (s[*i][0] == '|')
			break ;
		(*i)++;
	}
	n = malloc(sizeof(char *) * ((*i) - tmp) + 1);
	k = (*i) - tmp;
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

char	*find(char *ss, char c)
{
	int	i;

	i = 0;
	while (ss[i])
	{
		if (ss[i] == c || ss[i] == '\n')
			ss[i] = '\0';
		i++;
	}
	return (ss);
}

void	read_f(char *s, int fd, t_env **env)
{
	int		i;
	int		j;
	char	*str;
	int		l;
	char	*ss;
	char	*value;
	char	*newstr;

	i = 0;
	j = 0;
	ss = NULL;
	str = NULL;
	if (!s)
		return ;
	l = ft_strlen(s);
	write(1, "heredoc> ", 9);
	str = get_next_line(0);
	if (str == NULL || (ft_strncmp(s, str, l) == 1 && str[l] == '\n'))
		;
	else
	{
		while (1)
		{
			i = 0;
			if (find_dollar(str))
			{
				newstr = ft_strdup("");
				ss = ft_strdup(find_dollar(str));
				ss = find(ss, ' ');
				if (scan_list(ss, env))
				{
					value = scan_list(ss, env);
					while (i < ft_strlen(str))
					{
						if (str[i] == '$')
						{
							while (value[j])
							{
								newstr = charjoin(newstr, value[j]);
								j++;
							}
							while (str[i] && str[i] != ' ')
								i++;
							i--;
						}
						else
							newstr = charjoin(newstr, str[i]);
						i++;
					}
					newstr = charjoin(newstr, '\n');
				}
				else if (scan_list(ss, env) == NULL)
				{
					i++;
					newstr = ft_strdup(find(str, '$'));
					newstr = charjoin(newstr, '\n');
				}
			}
			if (i > 0)
				write(fd, newstr, ft_strlen(newstr));
			else
				write(fd, str, ft_strlen(str));
			free(str);
			str = NULL;
			write(1, "heredoc> ", 9);
			str = get_next_line(0);
			if (str == NULL || (ft_strncmp(s, str, l) == 1
					&& str[l] == '\n'))
				break ;
		}
	}
	close_read_f(fd, str);
}

char	*getfiles(char **s, char c, t_env **env)
{
	int		j;
	int		fd;
	char	*str;
	char	*name;

	j = 0;
	fd = 0;
	str = NULL;
	while (s[j])
	{
		if (s[j][0] == c && s[j][1] != c && s[j + 1][0] != c)
		{
			//str = ft_strdup("");
			if (str)
				free(str);
			str = NULL;
			str = ft_strdup(s[++j]);
			if (c == '<')
			{
				fd = open(str, O_RDONLY);
				if (fd == -1)
					perror("Error ");
			}
			else
				fd = open(str, O_CREAT | O_WRONLY, 0644);
			close(fd);
		}
		else if (s[j][0] == c && s[j][1] == c
				&& s[j][2] != c && s[j + 1][0] != c)
		{
			//str = ft_strdup("");
			if (str)
				free(str);
			str = NULL;
			str = ft_strdup(s[++j]);
			if (c == '>')
				fd = open(str, O_CREAT | O_WRONLY | O_APPEND, 0644);
			else
			{
				name = ft_strjoin(ft_strdup("/tmp/"), str);
				fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0777);
				// fd = open(str, O_CREAT | O_RDWR | O_TRUNC, 0777);
				read_f(str, fd, env);
				free(str);
				str = name;
			}
			close(fd);
		}
		j++;
	}
	return (str);
}

int	if_app(char **s, char *s1)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (ft_strcmp(s[i], s1) == 0)
			return (1);
		i++;
	}
	return (0);
}

t_cmd	*redirect_cmd(t_cmd *exec, char **s)
{
	t_redir		*cmdd;
	t_execcmd	*excmd;

	excmd = (t_execcmd *)exec;
	cmdd = malloc(sizeof(*cmdd));
	cmdd->type = '>';
	cmdd->app = 0;
	cmdd->herd = 0;
	if (excmd->infile && !if_app(s, "<<"))
		cmdd->infd = open(excmd->infile, O_RDONLY);
	else if (if_app(s, "<<"))
	{
		cmdd->infd = open(excmd->infile, O_RDWR, 0777);
		cmdd->herd = 1;
	}
	else
		cmdd->infd = -2;
	if (excmd->outfile && !if_app(s, ">>"))
		cmdd->outfd = open(excmd->outfile, O_CREAT | O_WRONLY, 0644);
	else if (if_app(s, ">>"))
	{
		cmdd->outfd = open(excmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
		cmdd->app = 1;
	}
	else
		cmdd->outfd = -2;
	cmdd->cmdn = exec;
	return ((t_cmd *)cmdd);
}

t_cmd	*pipecmd(t_cmd *left, t_cmd *right)
{
	t_ppipe	*cmdd;

	cmdd = malloc(sizeof(*cmdd));
	cmdd->type = '|';
	cmdd->left = left;
	cmdd->right = right;
	return ((t_cmd *) cmdd);
}

t_cmd	*execnode(char **s, char **envp, t_env **env)
{
	t_execcmd	*cmdd;

	cmdd = malloc(sizeof(*cmdd));
	cmdd->type = ' ';
	cmdd->argv = scan_arg(s);
	cmdd->infile = getfiles(s, '<', env);
	cmdd->outfile = getfiles(s, '>', env);
	cmdd->path = get_path(envp);
	cmdd = (t_execcmd *)redirect_cmd((t_cmd *)cmdd, s);
	return ((t_cmd *) cmdd);
}

t_cmd	*parce_pipe(char **str, int *i, char **envp, t_env **env)
{
	t_cmd	*cmdd;
	char	**tmp;

	if (checker(str, '|', i))
	{
		tmp = parceline(str, i);
		cmdd = pipecmd(execnode(tmp, envp, env),
				parce_pipe(str, i, envp, env));
	}
	else
	{
		tmp = parceline(str, i);
		cmdd = execnode(tmp, envp, env);
	}
	free_all(tmp);
	return (cmdd);
}

t_cmd	*magic_time(char **s, int *i, char **envp, t_env **env)
{
	t_cmd	*cmdd;

	cmdd = parce_pipe(s, i, envp, env);
	return ((t_cmd *) cmdd);
}

void	runcmd(t_cmd *cmdd, t_env **env, t_env **exportt, int *c)
{
	t_ppipe		*pcmd;
	t_execcmd	*execcmdd;
	t_redir		*rcmd;
	int			i;
	int			j;
	int			exstatus;
	char		*str;
	char		*temp;
	int			pp[2];

	i = 0;
	j = 1;
	if (cmdd->type == '|')
	{
		//printf("***********PIPE*************\n");
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
	else if (cmdd->type == ' ')
	{
		execcmdd = (t_execcmd *)cmdd;
		builtins(execcmdd->argv, env);
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
		exit(1);
	}
	else if (cmdd->type == '>')
	{
		//printf("****  REDERIC  ******\n");
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

void	handlle(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
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

int	main(int argc, char **argv, char **envp)
{
	int		i;
	int		c;
	char	*line;
	char	**str;
	t_cmd	*cmd;
	t_env	*env;
	t_env	*exportt;

	(void) argv;
	i = 0;
	env = NULL;
	exportt = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handlle);
	if (argc == 1)
	{
		createnv(&env, envp);
		createnv(&exportt, envp);
		while (1)
		{
			i = 0;
			line = readline("𝖒𝖎𝖓𝖎𝖘𝖍𝖊𝖑𝖑➜ ");
			signal(SIGINT, handlle);
			if (!line)
				return (0);
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
			//printf("%s\n",line);
			str = ft_split(line, ' ');
			undo(str);
			// int	f=0;
			// while (str[f])
			// {
			// 	printf("%s\n",str[f]);
			// 	f++;
			// }
			if (!checkerrors(str))
			{
				free(line);
				line = NULL;
				free_all(str);
				continue ;
			}
			c = 0;
			cmd = magic_time(str, &i, envp, &env);
			runcmd(cmd, &env, &exportt, &c);
			free(line);
			line = NULL;
			free_all(str);
			c = 0;
			free_tree(cmd, &c);
		}
	}
	return (0);
}
