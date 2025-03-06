/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:02:34 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/05 15:02:50 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_heredoc_content(t_shell *shell, char *delimiter, int fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, heredoc_signal_handler);
		process_heredoc_lines(shell, delimiter, fd);
	}
	waitpid(pid, &status, 0);
	close(fd);
	return (WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

int	setup_heredoc_read(t_shell *shell, t_heredoc_entry *entry, char *temp_file)
{
	int	fd;

	debug_print(0, 8, "Opening heredoc for reading: %s\n", temp_file);
	fd = open(temp_file, O_RDONLY);
	if (fd == -1)
	{
		unlink(temp_file);
		shell_free(shell, temp_file);
		return (1);
	}
	debug_print(0, 8, "Heredoc fd: %d\n", fd);
	entry->fd = fd;
	shell->heredoc.temp_file = temp_file;
	entry->next = shell->heredoc.entries;
	shell->heredoc.entries = entry;
	shell->heredoc.count++;
	return (0);
}

int	handle_heredoc(t_shell *shell, char *delimiter)
{
	char			*temp_file;
	int				fd;
	t_heredoc_entry	*entry;
	int				success;

	if (setup_heredoc_file(shell, &temp_file, &fd))
		return (1);
	entry = setup_heredoc_entry(shell, delimiter, temp_file, &fd);
	if (!entry)
		return (1);
	success = read_heredoc_content(shell, delimiter, fd);
	if (!success)
	{
		unlink(temp_file);
		shell_free(shell, temp_file);
		return (1);
	}
	return (setup_heredoc_read(shell, entry, temp_file));
}

int	get_heredoc_fd(t_shell *shell, char *delimiter)
{
	t_heredoc_entry	*current;

	current = shell->heredoc.entries;
	while (current)
	{
		if (ft_strcmp(current->delimiter, delimiter) == 0)
			return (current->fd);
		current = current->next;
	}
	return (-1);
}
