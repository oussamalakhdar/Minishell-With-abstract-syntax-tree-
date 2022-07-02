/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:51:01 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 14:54:41 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
