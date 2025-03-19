/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:36:06 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/19 16:42:02 by yutsong          ###   ########.fr       */
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
	t_redirection	*current;
	int				input_fd;
	int				output_fd;
	int				heredoc_fd;

	if (!redirs)
		return (0);
	if (backup_original_fds(shell))
	{
		cleanup_backup_fds(shell);
		return (1);
	}

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			input_fd = open(current->filename, O_RDONLY);
			if (input_fd == -1)
			{
				write(2, "minishell: ", 11);
				write(2, current->filename, ft_strlen(current->filename));
				write(2, ": No such file or directory\n", 28);
				restore_io(shell);
				return (1);
			}
			if (dup2(input_fd, STDIN_FILENO) == -1)
			{
				close(input_fd);
				restore_io(shell);
				return (1);
			}
			close(input_fd);
		}
		else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			output_fd = open_output_file(current);
			if (output_fd == -1)
			{
				printf("minishell: %s: Permission denied\n", current->filename);
				restore_io(shell);
				return (1);
			}
			if (dup2(output_fd, STDOUT_FILENO) == -1)
			{
				close(output_fd);
				restore_io(shell);
				return (1);
			}
			close(output_fd);
		}
		else if (current->type == REDIR_HEREDOC)
		{
			heredoc_fd = get_heredoc_fd(shell, current->filename);
			if (heredoc_fd == -1)
			{
				restore_io(shell);
				return (1);
			}
			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
			{
				close(heredoc_fd);
				restore_io(shell);
				return (1);
			}
			close(heredoc_fd);
		}
		current = current->next;
	}
	return (0);
}
