/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 07:59:23 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 08:29:39 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		debug_print(2047, 4, "  ");
		i++;
	}
}

void	print_redirection(t_redirection *redir)
{
	while (redir)
	{
		switch (redir->type)
		{
			case REDIR_IN:
				debug_print(2047, 4, "< %s", redir->filename);
				break ;
			case REDIR_OUT:
				debug_print(2047, 4, "> %s", redir->filename);
				break ;
			case REDIR_APPEND:
				debug_print(2047, 4, ">> %s", redir->filename);
				break ;
			case REDIR_HEREDOC:
				debug_print(2047, 4, "<< %s", redir->filename);
				break ;
			default :
				break ;
		}
		if (redir->next)
			debug_print(2047, 4, " ");
		redir = redir->next;
	}
}

void	print_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		debug_print(2047, 4, "%s", cmd->args[i]);
		if (cmd->args[i + 1])
			debug_print(2047, 4, " ");
		i++;
	}
	if (cmd->redirs)
	{
		debug_print(2047, 4, " ");
		print_redirection(cmd->redirs);
	}
	debug_print(2047, 4, "\n");
}

void	print_ast(t_ast_node *node, int depth)
{
	if (!node)
		return ;
	print_indent(depth);
	if (node->type == AST_COMMAND)
	{
		debug_print(2047, 4, "COMMAND: ");
		print_command(node->cmd);
	}
	else if (node->type == AST_PIPE)
	{
		debug_print(2047, 4, "PIPE:\n");
		print_ast(node->left, depth + 1);
		print_ast(node->right, depth + 1);
	}
}
