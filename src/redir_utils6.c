/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 01:00:08 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/21 01:06:23 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_input_error(t_shell *shell, t_redirection *redirs)
{
	int	has_input_error;

	has_input_error = handle_input_redirections(shell, redirs);
	if (has_input_error)
	{
		restore_io(shell);
		cleanup_backup_fds(shell);
		return (1);
	}
	return (0);
}

static int	print_input_error(t_redirection *redirs, t_shell *shell)
{
	t_redirection	*current;
	int				file_fd;

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
	return (0);
}

int	setup_redirections(t_shell *shell, t_redirection *redirs)
{
	if (!redirs)
		return (0);
	if (backup_original_fds(shell))
	{
		cleanup_backup_fds(shell);
		return (1);
	}
	if (shell->status.in_pipe == 0)
	{
		if (print_input_error(redirs, shell))
			return (1);
	}
	if (handle_output_redirections(shell, redirs))
	{
		cleanup_backup_fds(shell);
		return (1);
	}
	if (has_input_error(shell, redirs))
		return (1);
	return (0);
}
