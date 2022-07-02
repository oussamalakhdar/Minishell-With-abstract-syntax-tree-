/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:44:54 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 15:30:57 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
