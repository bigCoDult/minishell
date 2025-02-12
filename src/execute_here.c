/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/12 15:16:49 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 히어독 처리 함수
int handle_heredoc(t_shell *shell, char *delimiter)
{
    char *line;
    int pipe_fd[2];
    
    if (pipe(pipe_fd) == -1)
        return 1;

    // 히어독 입력 받기
    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }

    close(pipe_fd[1]);

    // 파이프의 읽기 끝을 저장
    shell->heredoc.pipe_fd[0] = pipe_fd[0];
    shell->heredoc.pipe_fd[1] = -1;
    shell->heredoc.delimiter = delimiter;

    return 0;
}