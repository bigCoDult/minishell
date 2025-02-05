/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:20 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 13:31:56 by yutsong          ###   ########.fr       */
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
    pid_t pid;
    int status;
    char *executable_path;
    char **env_array;

    printf("DEBUG: [execute_simple_command] Starting execution\n");
    printf("DEBUG: [execute_simple_command] Command args[0]: %s\n", cmd->args[0]);
    
    // 실행 파일 경로 찾기
    printf("DEBUG: [execute_simple_command] Finding executable path\n");
    executable_path = find_executable(shell, cmd->args[0]);
    if (!executable_path)
    {
        printf("DEBUG: [execute_simple_command] Executable not found\n");
        printf("%s: command not found\n", cmd->args[0]);
        return (127);
    }

    printf("DEBUG: [execute_simple_command] Creating environment array\n");
    env_array = get_env_array(shell);
    if (!env_array)
    {
        printf("DEBUG: [execute_simple_command] Failed to create env array\n");
        shell_free(shell, executable_path);
        return (1);
    }

    printf("DEBUG: [execute_simple_command] Forking process\n");
    pid = fork();
    if (pid == 0)
    {
        printf("DEBUG: [execute_simple_command] Child process started\n");
        printf("DEBUG: [execute_simple_command] Executing: %s\n", executable_path);
        if (execve(executable_path, cmd->args, env_array) == -1)
        {
            printf("DEBUG: [execute_simple_command] execve failed: %s\n", strerror(errno));
            exit(127);
        }
    }
    else if (pid < 0)
    {
        printf("DEBUG: [execute_simple_command] Fork failed\n");
        shell_free(shell, executable_path);
        int i = 0;
        while (env_array[i])
        {
            shell_free(shell, env_array[i]);
            i++;
        }
        shell_free(shell, env_array);
        return (1);
    }
    
    printf("DEBUG: [execute_simple_command] Parent waiting for child\n");
    waitpid(pid, &status, 0);
    
    printf("DEBUG: [execute_simple_command] Cleaning up resources\n");
    shell_free(shell, executable_path);
    int i = 0;
    while (env_array[i])
    {
        shell_free(shell, env_array[i]);
        i++;
    }
    shell_free(shell, env_array);

    if (WIFEXITED(status))
    {
        int exit_status = WEXITSTATUS(status);
        printf("DEBUG: [execute_simple_command] Command exited with status: %d\n", exit_status);
        return exit_status;
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
        printf("DEBUG: Executing command node: %s\n", node->cmd->args[0]);
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