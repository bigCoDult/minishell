/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:20 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_all_children(t_shell *shell, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		wait(&status);
		if (i == cmd_count - 1)
			shell->status.exit_code = WEXITSTATUS(status);
		i++;
	}
}

int	execute_simple_command(t_shell *shell, t_command *cmd)
{
	int				ret;
	t_redirection	*redir;

	debug_print(0, 7, "\n=== EXECUTE SIMPLE COMMAND START ===\n");
	debug_print(0, 7, "Command: %s\n", cmd->args[0]);
	debug_print(0, 7, "Current stdin FD: %d\n", STDIN_FILENO);
	redir = cmd->redirs;
	while (redir)
	{
		debug_print(0, 7, "Processing redirection type: %d\n", redir->type);
		debug_print(0, 7, "Redirection filename: %s\n", redir->filename);
		if (redir->type == REDIR_HEREDOC)
		{
			debug_print(0, 7, "Skipping already processed heredoc\n");
			redir = redir->next;
			continue ;
		}
		if (setup_redirections(shell, redir) != 0)
			return (1);
		redir = redir->next;
	}
	debug_print(0, 7, "Executing command: %s\n", cmd->args[0]);
	if (is_builtin(cmd->args[0]))
	{
		debug_print(0, 7, "Executing builtin command\n");
		ret = execute_builtin(shell, cmd);
	}
	else
	{
		debug_print(0, 7, "Executing external command\n");
		ret = execute_external(shell, cmd);
	}
	debug_print(0, 7, "Command execution completed with status: %d\n", ret);
	debug_print(0, 7, "Current stdin FD: %d\n", STDIN_FILENO);
	debug_print(0, 7, "=== EXECUTE SIMPLE COMMAND END ===\n\n");
	return (ret);
}

int	execute_ast(t_shell *shell, t_ast_node *node)
{
	t_redirection	*redir;

	if (!node)
		return (0);
	if (node->type == AST_COMMAND)
	{
		if (node->cmd->redirs)
		{
			redir = node->cmd->redirs;
			while (redir)
			{
				if (redir->type == REDIR_HEREDOC)
				{
					if (handle_heredoc(shell, redir->filename) != 0)
						return (1);
				}
				redir = redir->next;
			}
		}
		if (node->cmd->redirs)
		{
			redir = node->cmd->redirs;
			while (redir)
			{
				if (redir->type != REDIR_HEREDOC)
				{
					if (setup_redirections(shell, redir) != 0)
						return (1);
				}
				redir = redir->next;
			}
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
	debug_print(0, 4, "DEBUG: Starting command execution\n");
	if (!shell->ast_root)
	{
		debug_print(0, 4, "DEBUG: No AST root to execute\n");
		return (0);
	}
	result = execute_ast(shell, shell->ast_root);
	debug_print(0, 4, "DEBUG: Command execution completed with status: %d\n", result);
	return (result);
}
