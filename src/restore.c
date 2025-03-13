/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:57:09 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 07:03:43 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_fd(int fd)
{
	char	buffer[1];
	int		ret;

	ret = read(fd, buffer, 0);
	return (ret != -1);
}

static void	restore_fd(int *original_fd, int target_fd, const char *error_msg)
{
	if (*original_fd != -1)
	{
		if (is_valid_fd(*original_fd))
		{
			if (dup2(*original_fd, target_fd) == -1)
				perror(error_msg);
			close(*original_fd);
		}
		*original_fd = -1;
	}
}

void	restore_io(t_shell *shell)
{
	char	dummy;

	restore_fd(&shell->heredoc.original_stdin, STDIN_FILENO,
		"dup2 error while restoring stdin");
	restore_fd(&shell->original_stdout, STDOUT_FILENO,
		"dup2 error while restoring stdout");
	restore_fd(&shell->original_stderr, STDERR_FILENO,
		"dup2 error while restoring stderr");
	write(STDOUT_FILENO, &dummy, 0);
	write(STDERR_FILENO, &dummy, 0);
}
