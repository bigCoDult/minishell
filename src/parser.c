/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:12 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 05:35:28 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 새로운 명령어 노드 생성
static t_command *create_command(t_shell *shell)
{
    t_command *cmd;

    printf("DEBUG: Creating new command node\n");
    cmd = shell_malloc(shell, sizeof(t_command));
    if (!cmd)
    {
        printf("DEBUG: Failed to allocate command node\n");
        return (NULL);
    }

    cmd->name = NULL;
    cmd->args = NULL;
    cmd->redirs = NULL;
    cmd->pipe_next = NULL;
    printf("DEBUG: Command node created successfully\n");
    return (cmd);
}

// 명령어 인자 배열 생성
static char **create_args_array(t_shell *shell, t_token *start, int arg_count)
{
    char **args;
    int i;

    printf("DEBUG: Creating args array with count: %d\n", arg_count);
    args = shell_malloc(shell, sizeof(char *) * (arg_count + 1));
    if (!args)
    {
        printf("DEBUG: Failed to allocate args array\n");
        return (NULL);
    }

    i = 0;
    while (start && start->type == TOKEN_WORD && i < arg_count)
    {
        printf("DEBUG: Adding arg[%d]: %s\n", i, start->value);
        args[i] = shell_strdup(shell, start->value);
        if (!args[i])
        {
            printf("DEBUG: Failed to duplicate argument\n");
            return (NULL);
        }
        start = start->next;
        i++;
    }
    args[i] = NULL;
    printf("DEBUG: Args array created successfully\n");
    return (args);
}

// 토큰을 명령어로 파싱
static t_command *parse_command(t_shell *shell, t_token **curr_token)
{
    t_command *cmd;
    t_token *start;
    int arg_count;

    printf("DEBUG: Starting to parse command\n");
    if (!*curr_token)
    {
        printf("DEBUG: No tokens to parse\n");
        return (NULL);
    }

    cmd = create_command(shell);
    if (!cmd)
        return (NULL);

    // 명령어 이름과 인자 개수 세기
    start = *curr_token;
    arg_count = 0;
    while (*curr_token && (*curr_token)->type == TOKEN_WORD)
    {
        if (arg_count == 0)
        {
            printf("DEBUG: Found command name: %s\n", (*curr_token)->value);
            cmd->name = shell_strdup(shell, (*curr_token)->value);
        }
        arg_count++;
        *curr_token = (*curr_token)->next;
    }

    // 인자 배열 생성
    if (arg_count > 0)
    {
        cmd->args = create_args_array(shell, start, arg_count);
        if (!cmd->args)
        {
            printf("DEBUG: Failed to create args array\n");
            return (NULL);
        }
    }

    // 파이프 체크
    if (*curr_token && (*curr_token)->type == TOKEN_PIPE)
    {
        printf("DEBUG: Found pipe, parsing next command\n");
        *curr_token = (*curr_token)->next;  // 파이프 토큰 건너뛰기
        cmd->pipe_next = parse_command(shell, curr_token);
        if (!cmd->pipe_next)
        {
            printf("DEBUG: Failed to parse command after pipe\n");
            return (NULL);
        }
    }

    printf("DEBUG: Command parsing completed successfully\n");
    return (cmd);
}

// 기존 토큰 리스트 초기화
static void clear_tokens(t_shell *shell)
{
    t_token *current;
    t_token *next;

    printf("DEBUG: Clearing previous tokens\n");
    current = shell->tokens;
    while (current)
    {
        next = current->next;
        shell_free(shell, current->value);
        shell_free(shell, current);
        current = next;
    }
    shell->tokens = NULL;
}

// 기존 명령어 구조 초기화
static void clear_commands(t_shell *shell)
{
    t_command *current;
    t_command *next;

    printf("DEBUG: Clearing previous commands\n");
    current = shell->commands;
    while (current)
    {
        next = current->pipe_next;
        if (current->args)
        {
            for (int i = 0; current->args[i]; i++)
                shell_free(shell, current->args[i]);
            shell_free(shell, current->args);
        }
        shell_free(shell, current->name);
        // TODO: free redirs if needed
        shell_free(shell, current);
        current = next;
    }
    shell->commands = NULL;
}

int parse_input(t_shell *shell)
{
    t_token *curr_token;

    printf("DEBUG: Starting input parsing\n");
    
    // 이전 데이터 초기화
    clear_tokens(shell);
    clear_commands(shell);
    
    // 토큰화
    if (tokenize_input(shell) != 0)
    {
        printf("DEBUG: Tokenization failed\n");
        return (1);
    }
    printf("DEBUG: Tokenization completed\n");

    // 토큰이 없으면 종료
    if (!shell->tokens)
    {
        printf("DEBUG: No tokens to parse\n");
        return (1);
    }

    // 토큰을 명령어 구조로 파싱
    curr_token = shell->tokens;
    shell->commands = parse_command(shell, &curr_token);
    
    if (!shell->commands)
    {
        printf("DEBUG: Command parsing failed\n");
        return (1);
    }

    printf("DEBUG: Input parsing completed successfully\n");
    return (0);
}