/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:20 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/10 02:13:50 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_simple_command(t_shell *shell, t_command *cmd)
{
	int	ret;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
	{
		if (setup_redirections(shell, cmd->redirs) != 0)
			return (1);
		ret = execute_builtin(shell, cmd);
		restore_io(shell);
		if (shell->heredoc.original_stdin != -1 || shell->original_stdout != -1)
			restore_io(shell);
	}
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
	int	stdout_backup;
	int	devnull_fd;

	g_signal = 0;
	if (!shell->ast_root)
		return (0);
	if (shell->heredoc.original_stdin != -1 || shell->original_stdout != -1)
		restore_io(shell);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdout_backup == -1)
		return (1);
	devnull_fd = open("/dev/null", O_WRONLY);
	if (devnull_fd == -1)
	{
		close(stdout_backup);
		return (1);
	}
	dup2(devnull_fd, STDOUT_FILENO);
	close(devnull_fd);
	int heredoc_result = handle_all_heredocs(shell, shell->ast_root);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	if (heredoc_result != 0)
	{
		restore_io(shell);
		return (1);
	}
	setup_signals_executing();
	result = execute_ast(shell, shell->ast_root);
	restore_io(shell);
	setup_signals_interactive();
	return (result);
}
