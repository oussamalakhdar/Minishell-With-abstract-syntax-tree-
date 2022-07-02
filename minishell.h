/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 11:19:14 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 14:05:19 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <signal.h>
# include "getnextline/get_next_line.h"

typedef struct s_env
{
	char			*var_name;
	char			*var_value;
	int				flag;
	struct s_env	*next;
}	t_env;

typedef struct s_ex
{
	char	**var;
	char	*value;
	t_env	*new;
	t_env	*new2;
	int		flag;
	char	c;
}	t_ex;

enum e_def
{
	NL = 18,
	QUOT = 19,
	QUOT2 = 20,
	OUT = 21,
	IN = 22,
	SPACE2 = 23,
	PIPE = 24,
	REDR = 25,
	REDL = 26,
}	;

typedef struct s_cmd
{
	int	type;
}	t_cmd;

typedef struct s_execcmd
{
	int		type;
	char	**path;
	char	**argv;
	char	*infile;
	char	*outfile;
}	t_execcmd;

typedef struct s_pipe
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}	t_ppipe;

typedef struct s_redir
{
	int					type;
	int					app;
	int					herd;
	int					infd;
	int					outfd;
	struct s_cmd		*cmdn;
}	t_redir;

int	g_status;

t_env	*ft_lstnew(char *name, char *value);
void	ft_lstadd_back(t_env **lst, t_env *new);
void	createnv(t_env **env, char **envp);
char	**ft_split(char const *s, char c);
int		ft_strlen(char *str);
int		ft_strncmp(char *s1, char *s2, int n);
char	*putspace(char *s, t_env **env);
char	*ft_strdup(const char *s1);
char	*charjoin(char *s, char c);
char	*remove_space(char *s, char c, int *b);
void	undo(char **s);
void	free_all(char **s);
int		ft_strcmp(char *s1, char *s2);
int		myfork(void);
void	putinfile(int fd, char *s);
void	close_read_f(int fd, char *str);
char	*pwd(void);
int		scanner(char *s);
int		tablen(char **str);
void	builtins(char **s, t_env **env);
char	check_cmd(char *s);
void	printenv(t_env **env, char *s);
void	export(char **s, t_env **env, t_env **export);
void	printexport(t_env **export, char *s);//, t_ex *data);
void	unset(char *s, t_env **env);
char	*scan_list(char *s, t_env **node);
void	handlle(int sig);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_itoa(int n);
char	*find_dollar(char *s);
char	*ft_strjoin(char *s1, char *s2);
char	**get_path(char **env, t_env *envp);
char	**scan_arg(char **s);
int		chrr(char **s, int *i);
int		checker(char **s, char c, int *i);
int		is_specialchar(char s);
char	*pwd(void);
void	echo(char **s);
int		helper_scanner(char *c, char q, int *i);
char	*add_value(char *s, t_env **env);
void	add_value_utils2(char *s, int i);
int		add_value_utils3(char *s, int i, char **ret);
int		add_value_utils4(char **name, char **ret, char *s, int i);
char	*cut_value(char *str);
t_cmd	*redirect_cmd(t_cmd *exec, char **s);
t_cmd	*pipecmd(t_cmd *left, t_cmd *right);
t_cmd	*execnode(char **s, char **envp, t_env **env);
t_cmd	*parce_pipe(char **str, int *i, char **envp, t_env **env);
t_cmd	*magic_time(char **s, int *i, char **envp, t_env **env);
int		is_degit(char *s);
int		check_exit(char **s);
int		checkerrors(char **s);
int		checkerrors_extra1(char **s, int i);
int		checkerrors_extra2(char **s, int i);
char	**parceline(char **s, int *i);
char	*find(char *ss, char c);
void	read_f(char *s, int fd, t_env **env);
char	*getfiles(char **s, char c, t_env **env);
int		if_app(char **s, char *s1);
int		find_space(char *s);
char	*read_f_utils(char *str, t_env **env, int *i);
void	read_file(char	**str, t_env **env, int fd, char *s);
char	*putspace_utils(char *s, char *str, int i);
char	*remove_space_utils(char *s, char c, char *str, int i);
int		add_value_utils7(char **ret, char *s, int *i, t_env **env);
void	free_and_wite(int i, int fd, char *newstr, char *tmp);
void	rednorm(t_redir *cmdd, char *s);
void	get_file_utils(char **str, char **s, char c, int *j);
void	get_file_utils2(char **str, char **s, t_ex *data, int *j);
void	runcmdpipe(t_cmd *cmdd, int *c, t_env **env, t_env **exportt);
void	runcmd(t_cmd *cmdd, t_env **env, t_env **exportt, int *c);
void	runcmdexec(t_cmd *cmdd, t_env **env);
void	run_mid_cmd(t_redir *r, int *c, t_env **env, t_env **exportt);
void	run_lastcmd(t_env **env, t_env **exportt, t_redir *rcmd, int *c);
void	run_firstone(t_env **env, t_env **exportt, t_redir *rcmd, int *c);
void	run_cmd_first_utils(t_redir *r, t_env **env, t_env **exportt, int *c);
void	run_motherfucker_run(t_redir *rcmd, t_env **env, t_env **t, int *c);
void	run_left_pipe(t_ppipe *pcmd, int *c, t_ex *data, int *pp);
void	run_pp_utils(int pid, int *pp);
void	run_exec_utils(t_execcmd *execcmdd);
void	run_run_utils(t_execcmd *execcmdd);

#endif