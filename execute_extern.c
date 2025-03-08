static void	execute_command_in_child(t_shell *shell, t_command *cmd)
{
	char	*path;
	int		heredoc_fd = -1;
	
	// 더 안전한 방식으로 fd 저장
	if (shell->heredoc.entries)
	{
		heredoc_fd = dup(shell->heredoc.entries->fd); // fd를 복제하여 원본이 닫혀도 안전하게 사용
		if (heredoc_fd == -1)
			heredoc_fd = shell->heredoc.entries->fd; // 복제 실패 시 원본 사용
	}

	if (cmd->redirs && setup_redirections(shell, cmd->redirs) != 0)
	{
		free_exit(shell, 1);
	}
	path = find_command_path(shell, cmd->args[0]);
	if (!path)
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		free_exit(shell, 127);
	}
	if (heredoc_fd != -1)
	{
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	execve(path, cmd->args, get_env_array(shell));
	free_exit(shell, 127);
} 