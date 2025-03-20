/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:20 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 06:12:24 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_simple_command(t_shell *shell, t_command *cmd)
{
	int	ret;
	int	redir_status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
	{
		redir_status = setup_redirections(shell, cmd->redirs);
		if (redir_status != 0)
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
	if (!redirs)
		return (0);
	if (setup_redirections(shell, redirs) != 0)
		return (1);
	return (0);
}
