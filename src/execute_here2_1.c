/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here2_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 03:00:36 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 07:07:46 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_null_line(t_shell *shell, int fd)
{
	write(STDERR_FILENO, "\n", 1);
	free_env(shell);
	free_shell_malloc(shell);
	close(fd);
	if (g_signal == SIGINT)
		exit(130);
	else
		exit(0);
}

void	check_delimiter_match(char *line, char *delimiter,
			int fd, t_shell *shell)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		close(fd);
		free_exit(shell, 0);
	}
}
