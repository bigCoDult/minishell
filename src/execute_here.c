/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/11 15:37:14 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 히어독 처리 함수 추가
int handle_heredoc(t_shell *shell, char *delimiter)
{
    char *line;
    int pipe_fd[2];
    int original_stdin;
    
    debug_print(2047, 9, "DEBUG: [handle_heredoc] Starting with delimiter: %s\n", delimiter);
    
    // 현재 표준 입력 저장
    original_stdin = dup(STDIN_FILENO);
    if (original_stdin == -1)
    {
        debug_print(2047, 9, "DEBUG: [handle_heredoc] Failed to backup stdin\n");
        return (-1);
    }

    if (pipe(pipe_fd) == -1)
    {
        debug_print(2047, 9, "DEBUG: [handle_heredoc] Pipe creation failed\n");
        close(original_stdin);
        return (-1);
    }

    debug_print(2047, 9, "DEBUG: [handle_heredoc] Reading input until delimiter\n");
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            debug_print(2047, 9, "DEBUG: [handle_heredoc] EOF encountered\n");
            break;
        }

        debug_print(2047, 9, "DEBUG: [handle_heredoc] Read line: %s\n", line);
        if (strcmp(line, delimiter) == 0)
        {
            debug_print(2047, 9, "DEBUG: [handle_heredoc] Delimiter found, ending input\n");
            free(line);
            break;
        }

        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }

    debug_print(2047, 9, "DEBUG: [handle_heredoc] Closing write end\n");
    close(pipe_fd[1]);
    
    // 표준 입력을 파이프의 읽기 끝으로 변경
    if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
    {
        debug_print(2047, 9, "DEBUG: [handle_heredoc] dup2 failed\n");
        close(pipe_fd[0]);
        close(original_stdin);
        return (-1);
    }
    close(pipe_fd[0]);

    // 명령어 실행 후 표준 입력 복원
    shell->heredoc.original_stdin = original_stdin;  // 나중에 복원하기 위해 저장
    debug_print(2047, 9, "DEBUG: [handle_heredoc] Setup completed successfully\n");
    return (0);
}