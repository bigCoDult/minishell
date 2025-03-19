/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe7.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:50:52 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/19 10:56:51 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipe_stdin(int pipefd[2])
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 error in right command");
		close(pipefd[0]);
		return (1);
	}
	close(pipefd[0]);
	return (0);
}

static void	handle_pipe_node(t_shell *shell, t_ast_node *node)
{
	int	ret;

	// 파이프라인의 다음 단계 실행
	// 파이프라인의 각 명령어는 execute_left_command, execute_right_command, handle_command_node에서
	// "command not found" 에러를 처리하도록 이미 수정되었음
	ret = execute_pipe(shell, node);
	free_exit(shell, ret);
}

static void	handle_command_node(t_shell *shell, t_ast_node *node)
{
	int	ret;

	setup_right_command_io(shell, node->cmd);
	
	// 명령어가 없는 경우 처리
	if (!node->cmd->args || !node->cmd->args[0])
	{
		free_exit(shell, 0);
		return;
	}
	
	if (is_builtin(node->cmd->args[0]))
	{
		ret = execute_builtin(shell, node->cmd);
		free_exit(shell, ret);
	}
	else
	{
		// 명령어가 존재하지만 빌트인이 아닌 경우
		// 명령어 존재 여부 검증은 execute_ast에서 이미 수행했으므로,
		// 여기서는 에러 메시지 출력 없이 바로 실행 시도
		execute_external_command(shell, node->cmd);
	}
}

void	execute_right_command(t_shell *shell, t_ast_node *node, int pipefd[2])
{
	if (setup_pipe_stdin(pipefd))
		free_exit(shell, 1);
	if (node->right->type == AST_PIPE)
		handle_pipe_node(shell, node->right);
	else if (node->right->type == AST_COMMAND)
		handle_command_node(shell, node->right);
	write(STDERR_FILENO, "minishell: right command error\n", 31);
	free_exit(shell, 1);
}
