/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:57:05 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 14:58:56 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
