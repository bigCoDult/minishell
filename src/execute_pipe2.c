/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:32:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 04:25:50 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipe_in(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

static pid_t	execute_left_child(t_shell *shell,
	t_ast_node *node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork (left)");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		execute_left_command(shell, node, pipefd);
	return (pid);
}

static pid_t	execute_right_child(t_shell *shell, t_ast_node *node,
								int pipefd[2], pid_t pid1)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork (right)");
		close(pipefd[0]);
		close(pipefd[1]);
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
		return (-1);
	}
	if (pid == 0)
		execute_right_command(shell, node, pipefd);
	return (pid);
}

static int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (1);
}

int	execute_pipe(t_shell *shell, t_ast_node *node)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		check_redir_result;

	if (!node || !node->left || !node->right)
		return (1);
		
	// 왼쪽 명령어의 리다이렉션만 먼저 확인
	if (node->left->type == AST_COMMAND && node->left->cmd->redirs)
	{
		// 자식 프로세스를 생성하여 리다이렉션만 테스트
		check_redir_result = fork();
		if (check_redir_result == 0)
		{
			// 자식 프로세스에서는 리다이렉션 설정만 테스트
			if (setup_redirections(shell, node->left->cmd->redirs) != 0)
			{
				// 리다이렉션 설정 실패 시 종료
				free_exit(shell, 1);
			}
			// 리다이렉션 성공 시 정상 종료
			free_exit(shell, 0);
		}
		
		// 부모 프로세스에서는 자식의 결과를 기다림
		waitpid(check_redir_result, &status1, 0);
		
		// 리다이렉션 오류가 있으면 파이프 실행 중단
		if (WIFEXITED(status1) && WEXITSTATUS(status1) != 0)
		{
			// 실제 명령을 실행하여 오류 메시지만 출력
			check_redir_result = fork();
			if (check_redir_result == 0)
			{
				setup_redirections(shell, node->left->cmd->redirs);
				free_exit(shell, 1);
			}
			waitpid(check_redir_result, NULL, 0);
			return (1);
		}
	}
	
	// 파이프 생성 및 명령 실행
	if (setup_pipe_in(pipefd))
		return (1);
	pid1 = execute_left_child(shell, node, pipefd);
	if (pid1 == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	pid2 = execute_right_child(shell, node, pipefd, pid1);
	if (pid2 == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_for_children(pid1, pid2));
}
