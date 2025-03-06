/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:20 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 17:32:23 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_simple_command(t_shell *shell, t_command *cmd)
{
	int				ret;
	t_redirection	*redir;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		printf("Invalid command structure\n");
		return (1);
	}
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			redir = redir->next;
			continue ;
		}
		if (setup_redirections(shell, redir) != 0)
			return (1);
		redir = redir->next;
	}
	if (is_builtin(cmd->args[0]))
		ret = execute_builtin(shell, cmd);
	else
		ret = execute_external(shell, cmd);
	return (ret);
}

int	process_heredocs(t_shell *shell, t_redirection *redirs)
{
	t_redirection	*redir;

	redir = redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			if (handle_heredoc(shell, redir->filename) != 0)
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

int	process_redirection(t_shell *shell, t_redirection *redirs)
{
	t_redirection	*redir;

	redir = redirs;
	while (redir)
	{
		if (redir->type != REDIR_HEREDOC)
		{
			if (setup_redirections(shell, redir) != 0)
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

int	execute_ast(t_shell *shell, t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == AST_COMMAND)
	{
		if (node->cmd->redirs)
		{
			if (process_heredocs(shell, node->cmd->redirs) != 0)
				return (1);
			if (process_redirection(shell, node->cmd->redirs) != 0)
				return (1);
		}
		return (execute_simple_command(shell, node->cmd));
	}
	else if (node->type == AST_PIPE)
		return (execute_pipe(shell, node));
	return (1);
}

int	execute_commands(t_shell *shell)
{
	int	result;

	g_signal = 0;
	if (!shell->ast_root)
		return (0);
	setup_signals_executing();
	result = execute_ast(shell, shell->ast_root);
	setup_signals_interactive();
	return (result);
}
