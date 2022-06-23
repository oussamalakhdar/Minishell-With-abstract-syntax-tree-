/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 11:19:14 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/23 12:09:19 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef	MINISHELL_H
# define	MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include "getnextline/get_next_line.h"

typedef struct s_env
{
	char			*var_name;
	char			*var_value;
	int				flag;
	struct s_env	*next;
}	t_env;

enum def
{
	OUT = 21,
	IN = 22,
	SPACE = 23,
	PIPE = 24,
	REDR = 25,
	REDL = 26,
}	;

typedef struct s_cmd
{
	int	type;
}	cmd;

typedef struct s_execcmd
{
	int		type;
	char	**path;
	char	**argv;
	char	*infile;
	char	*outfile;
}	execcmd;

typedef struct s_pipe
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}	ppipe;

typedef struct s_redir
{
	int					type;
	int					app;
	int					herd;
	int					infd;
	int					outfd;
	struct s_cmd		*cmdn;
}	redir;

t_env	*ft_lstnew(char *name, char *value);
void	ft_lstadd_back(t_env **lst, t_env *new);
void	createnv(t_env **env, char **envp);
char	**ft_split(char const *s, char c);
int		ft_strlen(char *str);
int		ft_strncmp(char *s1, char *s2, int n);
char	*putspace(char *s);
char	*ft_strdup(const char *s1);
char	*charjoin(char *s, char c);
char	*remove_space(char *s, char c, int *j);
void	undo(char **s);
void	free_all(char **s);
int		ft_strcmp(char *s1, char *s2);
int		myfork(void);
void	putinfile(int fd, char *s);
void	close_read_f(int fd, char *str);
char	*pwd(void);
int		scanner(char *s);
int		tablen(char **str);
void	builtins(char **s, t_env **env, t_env **exportt);
char	check_cmd(char *s);
void	printenv(t_env **env, char *s);
void	export(char **s, t_env **env, t_env **export);
void	printexport(t_env **export, char *s);
void	unset(char *s, t_env **env);
int		scan_list(char **s, t_env **node);
int	ft_lstsize(t_env *lst);
//t_env	*ft_lstcopy(t_env *orig);
// char	*tab_join(char **s);

#endif