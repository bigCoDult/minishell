/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/12 13:03:15 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 히어독 처리 함수 추가
int handle_heredoc(t_shell *shell, char *delimiter)
{
    char *line;
    int pipe_fd[2];
    int original_stdin;
    
    debug_print(0, 1, "\n=== HANDLE HEREDOC START ===\n");
    debug_print(0, 1, "Delimiter: %s\n", delimiter);
    
    // 현재 표준 입력 저장
    original_stdin = dup(STDIN_FILENO);
    debug_print(0, 1, "Original stdin FD: %d\n", original_stdin);
    if (original_stdin == -1)
    {
        debug_print(0, 9, "DEBUG: [handle_heredoc] Failed to backup stdin\n");
        return (-1);
    }

    if (pipe(pipe_fd) == -1)
    {
        debug_print(0, 1, "Pipe creation failed\n");
        close(original_stdin);
        return (-1);
    }
    debug_print(0, 1, "Pipe FDs - read: %d, write: %d\n", pipe_fd[0], pipe_fd[1]);
    debug_print(0, 9, "DEBUG: [handle_heredoc] Reading input until delimiter\n");
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            debug_print(0, 9, "DEBUG: [handle_heredoc] EOF encountered\n");
            break;
        }

        debug_print(0, 9, "DEBUG: [handle_heredoc] Read line: %s\n", line);
        if (strcmp(line, delimiter) == 0)
        {
            debug_print(0, 9, "DEBUG: [handle_heredoc] Delimiter found, ending input\n");
            free(line);
            break;
        }

        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }

    debug_print(0, 9, "DEBUG: [handle_heredoc] Closing write end\n");
    close(pipe_fd[1]);
    
    debug_print(0, 1, "Current stdin FD before dup2: %d\n", STDIN_FILENO);
    // 표준 입력을 파이프의 읽기 끝으로 변경
    if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
    {
        debug_print(0, 9, "DEBUG: [handle_heredoc] dup2 failed\n");
        close(pipe_fd[0]);
        close(original_stdin);
        return (-1);
    }
    debug_print(0, 1, "New stdin FD after dup2: %d\n", STDIN_FILENO);
    close(pipe_fd[0]);

    // 명령어 실행 후 표준 입력 복원
    shell->heredoc.original_stdin = original_stdin;  // 나중에 복원하기 위해 저장
    debug_print(0, 9, "DEBUG: [handle_heredoc] Setup completed successfully\n");
    debug_print(0, 9, "Stored original_stdin in shell: %d\n", shell->heredoc.original_stdin);
    debug_print(0, 9, "=== HANDLE HEREDOC END ===\n\n");
    return (0);
}