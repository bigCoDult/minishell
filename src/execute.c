/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:20 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/12 13:27:59 by sanbaek          ###   ########.fr       */
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

// int execute_simple_command(t_shell *shell, t_command *cmd)
// {
//     pid_t pid;
//     int status;

//     (void)shell;
//     printf("DEBUG: Executing simple command: %s\n", cmd->name);
    
//     pid = fork();
//     if (pid == 0)
//     {
//         // 자식 프로세스
//         if (execvp(cmd->name, cmd->args) == -1)
//         {
//             // 명령어를 찾을 수 없을 때 에러 메시지 출력
//             printf("%s: command not found\n", cmd->name);
//             exit(127);  // command not found
//         }
//     }
//     else if (pid < 0)
//     {
//         // fork 실패
//         printf("DEBUG: Fork failed\n");
//         return (1);
//     }
    
//     // 부모 프로세스
//     waitpid(pid, &status, 0);
//     if (WIFEXITED(status))
//     {
//         int exit_status = WEXITSTATUS(status);
//         printf("DEBUG: Command exited with status: %d\n", exit_status);
//         return exit_status;
//     }
    
//     return (1);
// }

int execute_simple_command(t_shell *shell, t_command *cmd)
{
    int ret;

    debug_print(2047, 1, "\n=== EXECUTE SIMPLE COMMAND START ===\n");
    debug_print(2047, 1, "Command: %s\n", cmd->args[0]);
    debug_print(2047, 1, "Current stdin FD: %d\n", STDIN_FILENO);
    
    // 리다이렉션 처리
    t_redirection *redir = cmd->redirs;
    while (redir)
    {
        debug_print(2047, 1, "Processing redirection type: %d\n", redir->type);
        debug_print(2047, 1, "Redirection filename: %s\n", redir->filename);
        
        // 히어독은 이미 handle_all_heredocs에서 처리되었으므로 건너뜀
        if (redir->type == REDIR_HEREDOC)
        {
            debug_print(2047, 1, "Skipping already processed heredoc\n");
            redir = redir->next;
            continue;
        }
        
        // 다른 리다이렉션 처리
        if (setup_redirections(shell, redir) != 0)
            return (1);
        redir = redir->next;
    }

    // 명령어 실행
    debug_print(2047, 1, "Executing command: %s\n", cmd->args[0]);
    if (is_builtin(cmd->args[0]))
    {
        debug_print(2047, 1, "Executing builtin command\n");
        ret = execute_builtin(shell, cmd);
    }
    else
    {
        debug_print(2047, 1, "Executing external command\n");
        ret = execute_external(shell, cmd);
    }

    debug_print(2047, 1, "Command execution completed with status: %d\n", ret);
    debug_print(2047, 1, "Current stdin FD: %d\n", STDIN_FILENO);
    debug_print(2047, 1, "=== EXECUTE SIMPLE COMMAND END ===\n\n");
    return ret;
}

int execute_ast(t_shell *shell, t_ast_node *node)
{
    debug_print(2047, 4, "DEBUG: Executing AST node\n");
    
    if (!node)
    {
        debug_print(2047, 4, "DEBUG: No node to execute\n");
        return (0);
    }

    // 노드 타입에 따른 실행
    if (node->type == AST_COMMAND)
    {
        debug_print(2047, 4, "DEBUG: Executing command node: %s\n", node->cmd->args[0]);
        return (execute_simple_command(shell, node->cmd));
    }
    else if (node->type == AST_PIPE)
    {
        debug_print(2047, 4, "DEBUG: Executing pipe node\n");
        return (execute_pipe(shell, node));
    }

    debug_print(2047, 4, "DEBUG: Unknown node type\n");
    return (1);
}

// 메인 실행 함수 수정
int execute_commands(t_shell *shell)
{
    debug_print(2047, 4, "DEBUG: Starting command execution\n");
    
    if (!shell->ast_root)
    {
        debug_print(2047, 4, "DEBUG: No AST root to execute\n");
        return (0);
    }

    int result = execute_ast(shell, shell->ast_root);
    debug_print(2047, 4, "DEBUG: Command execution completed with status: %d\n", result);
    return (result);
}