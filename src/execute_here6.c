/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here6.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 07:07:48 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 13:41:22 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_heredoc_io(t_shell *shell, int io_fds[3])
{
	io_fds[0] = dup(STDOUT_FILENO);
	io_fds[1] = dup(STDERR_FILENO);
	if (io_fds[0] == -1 || io_fds[1] == -1)
	{
		if (io_fds[0] != -1)
			close(io_fds[0]);
		if (io_fds[1] != -1)
			close(io_fds[1]);
		handle_null_line(shell, io_fds[2]);
	}
}

static char	*read_heredoc_line(void)
{
	char	buffer[4096];
	char	*line;
	int		len;
	char	*newline;

	write(STDERR_FILENO, "> ", 2);
	line = NULL;
	len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	while (len > 0)
	{
		buffer[len] = '\0';
		newline = ft_strchr(buffer, '\n');
		if (newline)
		{
			*newline = '\0';
			line = ft_strdup(buffer);
			break ;
		}
	}
	return (line);
}

static int	process_line(t_shell *shell, char *line,
	char *delimiter, int io_fds[3])
{
	if (!line)
	{
		close(io_fds[0]);
		close(io_fds[1]);
		handle_null_line(shell, io_fds[2]);
		return (0);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		close(io_fds[0]);
		close(io_fds[1]);
		close(io_fds[2]);
		free_exit(shell, 0);
		return (1);
	}
	write(io_fds[2], line, ft_strlen(line));
	write(io_fds[2], "\n", 1);
	free(line);
	return (0);
}

static void	heredoc_main_loop(t_shell *shell, char *delimiter, int io_fds[3])
{
	char	*line;

	while (1)
	{
		check_heredoc_signal(io_fds[2]);
		line = read_heredoc_line();
		if (process_line(shell, line, delimiter, io_fds))
			break ;
	}
}

void	process_heredoc_lines(t_shell *shell, char *delimiter, int fd)
{
	int	io_fds[3];

	io_fds[2] = fd;
	check_heredoc_signal(fd);
	setup_heredoc_io(shell, io_fds);
	heredoc_main_loop(shell, delimiter, io_fds);
	close(io_fds[0]);
	close(io_fds[1]);
}
