/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abayar <abayar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 11:19:14 by olakhdar          #+#    #+#             */
/*   Updated: 2022/06/14 19:26:54 by abayar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef	MINISHELL_H
# define	MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_env
{
	char			*var_name;
	char			*var_value;
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
	int				type;
	int				infd;
	int				outfd;
	struct s_cmd		*cmdn;
}	redir;

t_env	*ft_lstnew(char *name, char *value);
void	ft_lstadd_back(t_env **lst, t_env *new);
void    createnv(t_env **env, char **envp);
char	**ft_split(char const *s, char c);
int		ft_strlen(char *str);
int		ft_strncmp(char *s1, char *s2, int n);
char	*putspace(char *s);
char	*ft_strdup(const char *s1);
char	*charjoin(char *s, char c);
char	*remove_space(char *s);
void	undo(char **s);

#endif