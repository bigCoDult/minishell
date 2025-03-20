/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:36:06 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 15:03:24 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	backup_original_fds(t_shell *shell)
{
	if (shell->heredoc.original_stdin == -1)
		shell->heredoc.original_stdin = dup(STDIN_FILENO);
	if (shell->original_stdout == -1)
		shell->original_stdout = dup(STDOUT_FILENO);
	if (shell->original_stderr == -1)
		shell->original_stderr = dup(STDERR_FILENO);
	if (shell->heredoc.original_stdin == -1
		|| shell->original_stdout == -1
		|| shell->original_stderr == -1)
		return (1);
	return (0);
}

void	cleanup_backup_fds(t_shell *shell)
{
	if (shell->heredoc.original_stdin != -1)
	{
		close(shell->heredoc.original_stdin);
		shell->heredoc.original_stdin = -1;
	}
	if (shell->original_stdout != -1)
	{
		close(shell->original_stdout);
		shell->original_stdout = -1;
	}
	if (shell->original_stderr != -1)
	{
		close(shell->original_stderr);
		shell->original_stderr = -1;
	}
}

int	open_output_file(t_redirection *redir)
{
	int	fd;

	if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	setup_redirections(t_shell *shell, t_redirection *redirs)
{
	int				has_input_error;
	int				file_fd;
	t_redirection	*current;

	if (!redirs)
		return (0);
	if (backup_original_fds(shell))
	{
		cleanup_backup_fds(shell);
		return (1);
	}
	if (shell->status.in_pipe == 0)
	{
		current = redirs;
		while (current)
		{
			if (current->type == REDIR_IN)
			{
				file_fd = open(current->filename, O_RDONLY);
				if (file_fd == -1)
				{
					write(STDERR_FILENO, "minishell: ", 11);
					write(STDERR_FILENO,
						current->filename, ft_strlen(current->filename));
					write(STDERR_FILENO, ": No such file or directory\n", 28);
					shell->status.exit_code = 1;
					cleanup_backup_fds(shell);
					return (1);
				}
				close(file_fd);
			}
			current = current->next;
		}
	}
	if (handle_output_redirections(shell, redirs))
	{
		cleanup_backup_fds(shell);
		return (1);
	}
	has_input_error = handle_input_redirections(shell, redirs);
	if (has_input_error)
	{
		restore_io(shell);
		cleanup_backup_fds(shell);
		return (1);
	}
	return (0);
}
