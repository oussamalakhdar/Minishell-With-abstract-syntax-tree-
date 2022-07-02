/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_three.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olakhdar <olakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:19:23 by olakhdar          #+#    #+#             */
/*   Updated: 2022/07/02 15:21:16 by olakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tree_utils(t_cmd *tree)
{
	t_execcmd	*execnode;

	if (tree->type == ' ')
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

void	free_tree(t_cmd *tree, int	*r)
{
	t_ppipe		*pipenode;
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
	free_tree_utils(tree);
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
