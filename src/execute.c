/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:20 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/19 16:40:35 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_simple_command(t_shell *shell, t_command *cmd)
{
	int	ret;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
	{
		if (setup_redirections(shell, cmd->redirs) != 0)
			return (1);
		ret = execute_builtin(shell, cmd);
		restore_io(shell);
		if (shell->heredoc.original_stdin != -1 || shell->original_stdout != -1)
			restore_io(shell);
	}
	else
		ret = execute_external(shell, cmd);
	return (ret);
}

int	process_heredocs(t_shell *shell, t_redirection *redirs)
{
	t_redirection	*redir;

	redir = redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			if (handle_heredoc(shell, redir->filename) != 0)
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

int	process_redirection(t_shell *shell, t_redirection *redirs)
{
	if (!redirs)
		return (0);
	
	if (setup_redirections(shell, redirs) != 0)
		return (1);
	
	return (0);
}

// 파이프라인 내 모든 명령어 검증
static void	verify_command(t_shell *shell, t_command *cmd)
{
	char	*path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return;
		
	// 빌트인 명령어는 항상 존재하므로 검증 필요 없음
	if (is_builtin(cmd->args[0]))
		return;
		
	// 절대/상대 경로인 경우
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], F_OK) != 0)
			printf("minishell: %s: Nos such file or directory\n", cmd->args[0]);
		else if (access(cmd->args[0], X_OK) != 0)
			printf("minishell: %s: Permission denied\n", cmd->args[0]);
		return;
	}
	
	// PATH에서 검색해야 하는 경우
	path = find_command_path(shell, cmd->args[0]);
	if (!path)
		printf("minishell: %s: command not found\n", cmd->args[0]);
	else
		shell_free(shell, path);
}

static void	verify_commands_recursive(t_shell *shell, t_ast_node *node)
{
	if (!node)
		return;
		
	if (node->type == AST_COMMAND)
	{
		verify_command(shell, node->cmd);
	}
	else if (node->type == AST_PIPE)
	{
		// 파이프라인의 모든 명령어 검증
		verify_commands_recursive(shell, node->left);
		verify_commands_recursive(shell, node->right);
	}
}

int	execute_ast(t_shell *shell, t_ast_node *node)
{
	if (!node)
		return (0);
		
	// 파이프라인의 모든 명령어 사전 검증
	if (node->type == AST_PIPE)
		verify_commands_recursive(shell, node);
		
	if (node->type == AST_COMMAND)
	{
		if (node->cmd->redirs)
		{
			if (process_redirection(shell, node->cmd->redirs) != 0)
				return (1);
		}
		return (execute_simple_command(shell, node->cmd));
	}
	else if (node->type == AST_PIPE)
		return (execute_pipe(shell, node));
	return (1);
}
