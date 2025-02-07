/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:12 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 13:35:28 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 명령어 노드 생성
t_command *create_command(t_shell *shell, t_token **tokens)
{
    t_command *cmd;
    int arg_count;
    t_token *curr;

    printf("DEBUG: [create_command] Creating command from tokens\n");
    cmd = shell_malloc(shell, sizeof(t_command));
    if (!cmd)
    {
        printf("DEBUG: [create_command] Failed to allocate command\n");
        return (NULL);
    }

    // 초기화
    printf("DEBUG: [create_command] Initializing command structure\n");
    cmd->args = NULL;
    cmd->redirs = NULL;

    // 명령어 이름과 인자 개수 세기
    curr = *tokens;
    arg_count = 0;
    printf("DEBUG: [create_command] Counting arguments\n");
    while (curr && curr->type == TOKEN_WORD)
    {
        printf("DEBUG: [create_command] Processing token %d: %s\n",
               arg_count, curr->value);
        // if (arg_count == 0)
        // {
        //     cmd->args[0] = shell_strdup(shell, curr->value);
        //     printf("DEBUG: [create_command] Command name set to: %s\n", cmd->args[0]);
        // }
        arg_count++;
        curr = curr->next;
    }
    printf("DEBUG: [create_command] Found %d arguments\n", arg_count);

    // 인자 배열 생성
    if (arg_count > 0)
    {
        printf("DEBUG: [create_command] Allocating args array\n");
        cmd->args = create_args_array(shell, *tokens, arg_count);
        if (!cmd->args)
        {
            printf("DEBUG: [create_command] Failed to create args array\n");
            return (NULL);
        }
         // 인자 복사
        curr = *tokens;
        int i = 0;
        while (i < arg_count)
        {
            printf("DEBUG: [create_command] Copying argument %d: %s\n", 
                   i, curr->value);
            cmd->args[i] = shell_strdup(shell, curr->value);
            if (!cmd->args[i])
            {
                printf("DEBUG: [create_command] Failed to copy argument %d\n", i);
                return (NULL);
            }
            curr = curr->next;
            i++;
        }
        cmd->args[arg_count] = NULL;
        printf("DEBUG: [create_command] Arguments copied successfully\n");

        // 토큰 포인터 업데이트
        *tokens = curr;
    }
    else
    {
        printf("DEBUG: [create_command] No arguments found\n");
        return (NULL);
    }
    printf("DEBUG: [create_command] Command creation completed successfully\n");
    return (cmd);
}

// 명령어 인자 배열 생성
char **create_args_array(t_shell *shell, t_token *start, int arg_count)
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

// // 토큰을 명령어로 파싱
// static t_command *parse_command(t_shell *shell, t_token **curr_token)
// {
//     t_command *cmd;
//     t_token *start;
//     int arg_count;

//     printf("DEBUG: Starting to parse command\n");
//     if (!*curr_token)
//     {
//         printf("DEBUG: No tokens to parse\n");
//         return (NULL);
//     }

//     cmd = create_command(shell);
//     if (!cmd)
//         return (NULL);

//     // 명령어 이름과 인자 개수 세기
//     start = *curr_token;
//     arg_count = 0;
//     while (*curr_token && (*curr_token)->type == TOKEN_WORD)
//     {
//         if (arg_count == 0)
//         {
//             printf("DEBUG: Found command name: %s\n", (*curr_token)->value);
//             cmd->name = shell_strdup(shell, (*curr_token)->value);
//         }
//         arg_count++;
//         *curr_token = (*curr_token)->next;
//     }

//     // 인자 배열 생성
//     if (arg_count > 0)
//     {
//         cmd->args = create_args_array(shell, start, arg_count);
//         if (!cmd->args)
//         {
//             printf("DEBUG: Failed to create args array\n");
//             return (NULL);
//         }
//     }

//     // 파이프 체크
//     if (*curr_token && (*curr_token)->type == TOKEN_PIPE)
//     {
//         printf("DEBUG: Found pipe, parsing next command\n");
//         *curr_token = (*curr_token)->next;  // 파이프 토큰 건너뛰기
//         cmd->pipe_next = parse_command(shell, curr_token);
//         if (!cmd->pipe_next)
//         {
//             printf("DEBUG: Failed to parse command after pipe\n");
//             return (NULL);
//         }
//     }

//     printf("DEBUG: Command parsing completed successfully\n");
//     return (cmd);
// }

// // 기존 토큰 리스트 초기화
// static void clear_tokens(t_shell *shell)
// {
//     t_token *current;
//     t_token *next;

//     printf("DEBUG: Clearing previous tokens\n");
//     current = shell->tokens;
//     while (current)
//     {
//         next = current->next;
//         shell_free(shell, current->value);
//         shell_free(shell, current);
//         current = next;
//     }
//     shell->tokens = NULL;
// }

// // 기존 명령어 구조 초기화
// static void clear_commands(t_shell *shell)
// {
//     printf("DEBUG: Clearing previous commands\n");
//     if (shell->ast_root)
//     {
//         free_all_memory(shell);
//         shell->ast_root = NULL;
//     }
// }

// // AST 초기화 함수 추가
// static void clear_ast(t_shell *shell, t_ast_node *node)
// {
//     if (!node)
//         return;

//     printf("DEBUG: Clearing AST node\n");
//     free_all_memory(shell);
//     shell->ast_root = NULL;
// }

// 메인 파싱 함수
int parse_input(t_shell *shell)
{
    t_token *curr_token;

    printf("\nDEBUG: === Starting input parsing ===\n");

    // 이전 데이터 초기화
    if (shell->tokens)
    {
        free_command_memory(shell);
        shell->tokens = NULL;
    }
    if (shell->ast_root)
    {
        free_command_memory(shell);
        shell->ast_root = NULL;
    }
    
    if (tokenize_input(shell) != 0)
    {
        printf("DEBUG: Tokenization failed\n");
        return (1);
    }
    printf("DEBUG: Tokenization completed\n");

    // 토큰 목록 출력
    printf("\nDEBUG: Token list:\n");
    curr_token = shell->tokens;
    while (curr_token)
    {
        printf("DEBUG: Token type: %d, value: %s\n", 
               curr_token->type, curr_token->value);
        curr_token = curr_token->next;
    }

    curr_token = shell->tokens;
    shell->ast_root = parse_pipeline(shell, &curr_token);
    
    if (!shell->ast_root)
    {
        printf("DEBUG: Pipeline parsing failed\n");
        return (1);
    }

    printf("DEBUG: Parsing completed successfully\n");
    printf("DEBUG: === Parsing finished ===\n\n");
    return (0);
}