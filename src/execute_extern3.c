/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_extern3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 07:33:39 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 03:38:28 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_direct_paths(t_shell *shell, char *cmd_path)
{
	if (access(cmd_path, F_OK) != 0)
	{
		printf("minishell: %s: No such file or directory\n", cmd_path);
		free_exit(shell, 127);
	}
	else if (access(cmd_path, X_OK) != 0)
	{
		printf("minishell: %s: Permission denied\n", cmd_path);
		free_exit(shell, 126);
	}
	return (cmd_path);
}

static char	*handle_command_path(t_shell *shell, char *cmd_name, int heredoc_fd)
{
	char	*path;

	path = find_command_path(shell, cmd_name);
	if (!path)
	{
		if (heredoc_fd != -1)
			close(heredoc_fd);
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
		write(STDERR_FILENO, ": command not found\n", 20);
		free_exit(shell, 127);
	}
	return (path);
}

static char	*find_executable_path(t_shell *shell,
	t_command *cmd, int heredoc_fd)
{
	if (ft_strchr(cmd->args[0], '/'))
		return (handle_direct_paths(shell, cmd->args[0]));
	else
		return (handle_command_path(shell, cmd->args[0], heredoc_fd));
}

static void	setup_io_redirections(t_shell *shell,
	t_command *cmd, int heredoc_fd)
{
	int				original_stdout;

	// 표준 출력 백업
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout == -1)
	{
		if (heredoc_fd != -1)
			close(heredoc_fd);
		perror("dup");
		free_exit(shell, 1);
	}

	// heredoc_fd는 더 이상 필요 없음 - setup_redirections에서 리다이렉션 처리
	if (heredoc_fd != -1)
		close(heredoc_fd);
		
	// 모든 리다이렉션 처리 (히어독 및 일반 리다이렉션 포함)
	if (cmd->redirs && setup_redirections(shell, cmd->redirs) != 0)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
		free_exit(shell, 1);
	}

	// 백업 파일 디스크립터 닫기
	close(original_stdout);
}

void	execute_command_in_child(t_shell *shell, t_command *cmd)
{
	char	*path;
	int		heredoc_fd;

	heredoc_fd = find_command_heredoc_fd(shell, cmd);
	path = find_executable_path(shell, cmd, heredoc_fd);
	setup_io_redirections(shell, cmd, heredoc_fd);
	execve(path, cmd->args, get_env_array(shell));
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	free_exit(shell, 127);
}

