/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:20 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 02:12:17 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void wait_all_children(t_shell *shell, int cmd_count)
{
    int i;
    int status;
    
    i = 0;
    while (i < cmd_count)
    {
        wait(&status);
        if (i == cmd_count - 1)  // 마지막 명령어의 상태 저장
            shell->status.exit_code = WEXITSTATUS(status);
        i++;
    }
}

static int execute_simple_command(t_shell *shell, t_command *cmd)
{
    pid_t pid;
    int status;

    (void)shell;
    pid = fork();
    if (pid == 0)
    {
        // 자식 프로세스
        execvp(cmd->name, cmd->args);
        exit(127);  // 명령어를 찾을 수 없음
    }
    else if (pid > 0)
    {
        // 부모 프로세스
        waitpid(pid, &status, 0);
        return (WEXITSTATUS(status));
    }
    return (1);
}

int execute_ast(t_shell *shell, t_ast_node *node)
{
    printf("DEBUG: Executing AST node\n");
    
    if (!node)
    {
        printf("DEBUG: No node to execute\n");
        return (0);
    }

    // 노드 타입에 따른 실행
    if (node->type == AST_COMMAND)
    {
        printf("DEBUG: Executing command node: %s\n", node->cmd->name);
        return (execute_simple_command(shell, node->cmd));
    }
    else if (node->type == AST_PIPE)
    {
        printf("DEBUG: Executing pipe node\n");
        return (execute_pipe(shell, node));
    }

    printf("DEBUG: Unknown node type\n");
    return (1);
}

// 메인 실행 함수 수정
int execute_commands(t_shell *shell)
{
    printf("DEBUG: Starting command execution\n");
    
    if (!shell->ast_root)
    {
        printf("DEBUG: No AST root to execute\n");
        return (0);
    }

    int result = execute_ast(shell, shell->ast_root);
    printf("DEBUG: Command execution completed with status: %d\n", result);
    return (result);
}