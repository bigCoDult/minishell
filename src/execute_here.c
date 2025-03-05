/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/21 05:22:46 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_temp_heredoc_file(t_shell *shell)
{
	static int	count;
	char		*filename;
	char		num[32];

	count = 0;
	sprintf(num, "%d", count++);
	filename = shell_malloc(shell, ft_strlen("/tmp/heredoc_") + ft_strlen(num) + 1);
	if (!filename)
		return (NULL);
	strcpy(filename, "/tmp/heredoc_");
	strcat(filename, num);
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

int	handle_heredoc(t_shell *shell, char *delimiter)
{
	char			*temp_file;
	char			*line;
	int				fd;
	pid_t			pid;
	int				status;
	t_heredoc_entry	*entry;

	debug_print(2047, 8, "Creating heredoc for delimiter: %s\n", delimiter);
	temp_file = create_temp_heredoc_file(shell);
	if (!temp_file)
		return (1);
	debug_print(2047, 8, "Opening temp file: %s\n", temp_file);
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		shell_free(shell, temp_file);
		return (1);
	}
	entry = create_heredoc_entry(shell, delimiter);
	if (!entry)
	{
		close(fd);
		unlink(temp_file);
		shell_free(shell, temp_file);
		return (1);
	}
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, heredoc_signal_handler);
		while (1)
		{
			line = readline("> ");
			if (!line || strcmp(line, delimiter) == 0)
			{
				free(line);
				close(fd);
				free_exit(shell, 0);
			}
			debug_print(2047, 8, "Writing to heredoc: %s\n", line);
			write(fd, line, strlen(line));
			write(fd, "\n", 1);
			free(line);
		}
	}
	waitpid(pid, &status, 0);
	close(fd);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
	{
		debug_print(2047, 8, "Opening heredoc for reading: %s\n", temp_file);
		fd = open(temp_file, O_RDONLY);
		if (fd == -1)
		{
			unlink(temp_file);
			shell_free(shell, temp_file);
			return (1);
		}
		debug_print(2047, 8, "Heredoc fd: %d\n", fd);
		entry->fd = fd;
		shell->heredoc.temp_file = temp_file;
		entry->next = shell->heredoc.entries;
		shell->heredoc.entries = entry;
		shell->heredoc.count++;
		return (0);
	}
	unlink(temp_file);
	shell_free(shell, temp_file);
	return (1);
}

int	get_heredoc_fd(t_shell *shell, char *delimiter)
{
	t_heredoc_entry	*current;

	current = shell->heredoc.entries;
	while (current)
	{
		if (strcmp(current->delimiter, delimiter) == 0)
			return (current->fd);
		current = current->next;
	}
	return (-1);
}

void	cleanup_heredoc(t_shell *shell)
{
	t_heredoc_entry	*current;
	t_heredoc_entry	*next;

	current = shell->heredoc.entries;
	while (current)
	{
		next = current->next;
		if (current->fd != -1)
		{
			close(current->fd);
		}
		shell_free(shell, current->delimiter);
		shell_free(shell, current);
		current = next;
	}
	shell->heredoc.entries = NULL;
	shell->heredoc.count = 0;
	if (shell->heredoc.original_stdin != -1)
	{
		dup2(shell->heredoc.original_stdin, STDIN_FILENO);
		close(shell->heredoc.original_stdin);
		shell->heredoc.original_stdin = -1;
	}
	if (shell->heredoc.temp_file)
	{
		unlink(shell->heredoc.temp_file);
		shell_free(shell, shell->heredoc.temp_file);
		shell->heredoc.temp_file = NULL;
	}
}

int	find_command_heredoc_fd(t_shell *shell, t_command *cmd)
{
	t_redirection	*redir;
	t_heredoc_entry	*entry;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			entry = shell->heredoc.entries;
			while (entry)
			{
				if (strcmp(entry->delimiter, redir->filename) == 0)
					return (entry->fd);
				entry = entry->next;
			}
		}
		redir = redir->next;
	}
	return (-1);
}

void	setup_command_heredoc(t_shell *shell, t_command *cmd)
{
	int	heredoc_fd;

	heredoc_fd = find_command_heredoc_fd(shell, cmd);
	if (heredoc_fd != -1)
	{
		lseek(heredoc_fd, 0, SEEK_SET);
		dup2(heredoc_fd, STDIN_FILENO);
	}
}
