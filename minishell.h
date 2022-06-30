/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 11:19:14 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/30 22:33:08 by abayar           ###   ########.fr       */
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
void	add_value_utils2(char *s, int i);
void	add_value_utils3(char *s, int *i, char *ret);
int	add_value_utils4(char *name, char *ret, char *s, int *i);

#endif