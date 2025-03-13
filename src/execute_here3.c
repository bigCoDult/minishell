/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:02:34 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 06:41:16 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_process(t_shell *shell, char *delimiter, int fd)
{
	struct termios	term;

	signal(SIGINT, heredoc_signal_handler);
	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag |= ECHO;
		term.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
	process_heredoc_lines(shell, delimiter, fd);
	exit(0);
}

static pid_t	setup_heredoc_process(t_shell *shell, int *original_g_signal)
{
	pid_t	pid;

	(void)shell;
	*original_g_signal = g_signal;
	g_signal = 0;
	setup_signals_heredoc();
	pid = fork();
	if (pid < 0)
		perror("fork");
	return (pid);
}

static int	process_exit_status(t_shell *shell,
	int status, int original_g_signal)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_signal = SIGINT;
		shell->status.exit_code = 130;
		return (0);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		if (WEXITSTATUS(status) == 130)
		{
			g_signal = SIGINT;
			shell->status.exit_code = 130;
		}
		return (0);
	}
	g_signal = original_g_signal;
	return (WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

int	read_heredoc_content(t_shell *shell, char *delimiter, int fd)
{
	pid_t	pid;
	int		status;
	int		original_g_signal;

	status = 0;
	pid = setup_heredoc_process(shell, &original_g_signal);
	if (pid < 0)
		return (0);
	if (pid == 0)
		handle_child_process(shell, delimiter, fd);
	waitpid(pid, &status, 0);
	setup_signals_interactive();
	close(fd);
	return (process_exit_status(shell, status, original_g_signal));
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
