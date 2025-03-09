/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here2_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 03:00:36 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/09 12:59:09 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_null_line(t_shell *shell, int fd)
{
	free_env(shell);
	free_shell_malloc(shell);
	close(fd);
	if (g_signal == SIGINT)
		exit(130);
	else
		exit(0);
}

void	check_delimiter_match(char *line, char *delimiter, int fd, t_shell *shell)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		close(fd);
		free_exit(shell, 0);
	}
}

void	process_heredoc_lines(t_shell *shell, char *delimiter, int fd)
{
	char	*line;
	int		original_stdout;
	int		original_stderr;
	char	buffer[4096];
	int		len;

	(void)shell;
	check_heredoc_signal(fd);
	original_stdout = dup(STDOUT_FILENO);
	original_stderr = dup(STDERR_FILENO);
	if (original_stdout == -1 || original_stderr == -1)
	{
		if (original_stdout != -1)
			close(original_stdout);
		if (original_stderr != -1)
			close(original_stderr);
		handle_null_line(shell, fd);
	}
	while (1)
	{
		check_heredoc_signal(fd);
		write(STDERR_FILENO, "> ", 2);
		line = NULL;
		len = 0;
		while ((len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[len] = '\0';
			char *newline = ft_strchr(buffer, '\n');
			if (newline)
			{
				*newline = '\0';
				line = ft_strdup(buffer);
				break ;
			}
		}
		if (!line)
		{
			close(original_stdout);
			close(original_stderr);
			handle_null_line(shell, fd);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			close(original_stdout);
			close(original_stderr);
			close(fd);
			free_exit(shell, 0);
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);	
		free(line);
	}
	close(original_stdout);
	close(original_stderr);
}
