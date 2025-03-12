/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_extern.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:38:39 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/12 14:02:59 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_command_in_child(t_shell *shell, t_command *cmd)
{
	char	*path;
	int		heredoc_fd;
	int		original_stdout;

	heredoc_fd = -1;
	original_stdout = -1;
	heredoc_fd = find_command_heredoc_fd(shell, cmd);
	
	// 슬래시가 포함된 경로인지 확인
	if (ft_strchr(cmd->args[0], '/'))
	{
		// 직접 실행 시도
		if (access(cmd->args[0], F_OK) != 0)
		{
			// 파일이 존재하지 않음
			fprintf(stderr, "minishell: %s: No such file or directory\n", cmd->args[0]);
			free_exit(shell, 127);
		}
		else if (access(cmd->args[0], X_OK) != 0)
		{
			// 파일이 존재하지만 실행 권한이 없음
			fprintf(stderr, "minishell: %s: Permission denied\n", cmd->args[0]);
			free_exit(shell, 126);
		}
		path = cmd->args[0];
	}
	else
	{
		path = find_command_path(shell, cmd->args[0]);
		if (!path)
		{
			if (heredoc_fd != -1)
				close(heredoc_fd);
			fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
			free_exit(shell, 127);
		}
	}
	
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout == -1)
	{
		if (heredoc_fd != -1)
			close(heredoc_fd);
		perror("dup");
		free_exit(shell, 1);
	}
	if (cmd->redirs && setup_redirections(shell, cmd->redirs) != 0)
	{
		if (heredoc_fd != -1)
			close(heredoc_fd);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
		free_exit(shell, 1);
	}
	close(original_stdout);
	if (heredoc_fd != -1)
	{
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	execve(path, cmd->args, get_env_array(shell));
	fprintf(stderr, "minishell: %s: %s\n", cmd->args[0], strerror(errno));
	free_exit(shell, 127);
}

static int	handle_signal_termination(t_shell *shell, int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			g_signal = SIGINT;
			shell->status.exit_code = 130;
			return (130);
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			g_signal = SIGQUIT;
			shell->status.exit_code = 131;
			return (131);
		}
	}
	return (shell->status.exit_code);
}

int	execute_external(t_shell *shell, t_command *cmd)
{
	pid_t			pid;
	int				status;
	t_redirection	*redir;

	if (cmd->redirs)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
				fprintf(stderr, "Debug: Redirecting to file '%s'\n", redir->filename);
			redir = redir->next;
		}
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_command_in_child(shell, cmd);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->status.exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->status.exit_code = 128 + WTERMSIG(status);
		else
			shell->status.exit_code = 1;
		
		handle_signal_termination(shell, status);
		return (shell->status.exit_code);
	}
	return (1);
}
