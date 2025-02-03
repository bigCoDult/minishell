

void	execute(t_shell *shell)
{
	t_node *node;
	t_io_fd	in_fd;

	node = shell->node;
	io_fd.saved_stdin = dup(STDIN_FILENO);
	io_fd.saved_stdout = dup(STDOUT_FILENO);
	
	if (!node || !env_head)
		return ;
	if (node->token->type == PIPE)
		exe_pipe(node, env_head);
	exe_task(node, env_head);

	restore_io(io_fds.saved_stdout, io_fds.saved_stdin);
}

void exe_task(t_tree *node, t_env *env_head)
{
	exe_here(node, env_head);
	exe_rdir(node, env_head);
	exe_cmd(node, env_head);
}






void exe_pipe(t_shell *shell)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		exe_task(node->left, env_head);
		exit(0);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (node->right_sibling && node->right_sibling->token->type == PIPE)
			exe_pipe(node->right_sibling, env_head);
		else
		exe_task(node->right, env_head);
		waitpid(pid, &status, 0);
	}
}