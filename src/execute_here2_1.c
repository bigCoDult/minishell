/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here2_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 03:00:36 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/07 09:06:36 by yutsong          ###   ########.fr       */
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

void	check_delimiter_match(char *line, char *delimiter, int fd)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		close(fd);
		exit(0);
	}
}

void	process_heredoc_lines(t_shell *shell, char *delimiter, int fd)
{
	char	*line;

	(void)shell;
	check_heredoc_signal(fd);
	while (1)
	{
		check_heredoc_signal(fd);
		line = readline("> ");
		if (!line)
			handle_null_line(shell, fd);
		check_delimiter_match(line, delimiter, fd);
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}
