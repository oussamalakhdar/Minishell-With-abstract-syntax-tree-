/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 10:01:33 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 15:34:05 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		rednorm(cmdd, excmd->infile);
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
	cmdd->path = get_path(envp, *env);
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
