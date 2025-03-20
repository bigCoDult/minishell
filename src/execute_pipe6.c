/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe6.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:44:39 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 03:48:24 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_direct_path(t_shell *shell, char *cmd_path)
{
	if (access(cmd_path, F_OK) != 0 || access(cmd_path, X_OK) != 0)
	{
		// 파일이 존재하지 않거나 실행 권한이 없음
		// 에러 메시지는 execute_ast에서 이미 출력했으므로 여기서는 NULL 반환
		free_exit(shell, 127);
		return (NULL);
	}
	return (cmd_path);
}

char	*handle_path_search(t_shell *shell, char *cmd_name)
{
	char	*path;

	path = find_command_path(shell, cmd_name);
	if (!path)
	{
		// 명령어를 찾을 수 없음
		// 에러 메시지는 execute_ast에서 이미 출력했으므로 여기서는 NULL 반환
		free_exit(shell, 127);
		return (NULL);
	}
	return (path);
}

static void	run_command(t_shell *shell, char *path, char **args)
{
	execve(path, args, get_env_array(shell));
	printf("minishell: %s: %s\n", args[0], strerror(errno));
	free_exit(shell, 127);
}

void	execute_external_command(t_shell *shell, t_command *cmd)
{
	char	*path;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		// 명령어가 없는 경우
		free_exit(shell, 0);
		return;
	}
	
	// 명령어 존재 여부 검증은 execute_ast에서 이미 수행했으므로,
	// 여기서는 그대로 실행 시도
	if (ft_strchr(cmd->args[0], '/'))
		path = handle_direct_path(shell, cmd->args[0]);
	else
		path = handle_path_search(shell, cmd->args[0]);
	
	run_command(shell, path, cmd->args);
}
