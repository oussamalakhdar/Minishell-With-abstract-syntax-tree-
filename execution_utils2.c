/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:11:33 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 15:14:38 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
