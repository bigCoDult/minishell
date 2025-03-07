/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:01:47 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 17:57:09 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_temp_heredoc_file(t_shell *shell)
{
	static int	count;
	char		*filename;
	char		num[32];

	count = 0;
	// sprintf?
	sprintf(num, "%d", count++);
	filename = shell_malloc(shell, ft_strlen("/tmp/heredoc_")
			+ ft_strlen(num) + 1);
	if (!filename)
		return (NULL);
	ft_strcpy(filename, "/tmp/heredoc_");
	ft_strcat(filename, num);
	return (filename);
}

t_heredoc_entry	*create_heredoc_entry(t_shell *shell, char *delimiter)
{
	t_heredoc_entry	*entry;

	entry = shell_malloc(shell, sizeof(t_heredoc_entry));
	if (!entry)
		return (NULL);
	entry->delimiter = shell_strdup(shell, delimiter);
	if (!entry->delimiter)
	{
		shell_free(shell, entry);
		return (NULL);
	}
	entry->fd = -1;
	entry->next = NULL;
	return (entry);
}

int	setup_heredoc_file(t_shell *shell, char **temp_file, int *fd)
{
	*temp_file = create_temp_heredoc_file(shell);
	if (!*temp_file)
		return (1);
	*fd = open(*temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
	{
		shell_free(shell, *temp_file);
		return (1);
	}
	return (0);
}

t_heredoc_entry	*setup_heredoc_entry(t_shell *shell, char *delimiter
		, char *temp_file, int *fd)
{
	t_heredoc_entry	*entry;

	entry = create_heredoc_entry(shell, delimiter);
	if (!entry)
	{
		close(*fd);
		unlink(temp_file);
		shell_free(shell, temp_file);
		return (NULL);
	}
	return (entry);
}

void	process_heredoc_lines(t_shell *shell, char *delimiter, int fd)
{
	char	*line;

	(void)shell;
	if (g_signal == SIGINT)
	{
		close(fd);
		exit(130);
	}
	while (1)
	{
		if (g_signal == SIGINT)
		{
			close(fd);
			exit(130);
		}
		line = readline("> ");
		if (!line)
		{
			close(fd);
			if (g_signal == SIGINT)
				exit(130);
			else
				exit(0);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			close(fd);
			exit(0);
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}
