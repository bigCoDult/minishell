/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:12 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/12 13:27:59 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// // 토큰 타입을 리다이렉션 타입으로 변환하는 함수 추가
// static t_redirection_type get_redirection_type(const char *token_value)
// {
//     printf("DEBUG: [get_redirection_type] Token value: %s\n", token_value);
//     if (!token_value)
//         return (REDIR_NONE);
//     if (strcmp(token_value, "<") == 0)
//         return (REDIR_IN);
//     else if (strcmp(token_value, ">") == 0)
//         return (REDIR_OUT);
//     else if (strcmp(token_value, ">>") == 0)
//         return (REDIR_APPEND);
//     else if (strcmp(token_value, "<<") == 0)
//         return (REDIR_HEREDOC);
//     return (REDIR_NONE);  // 기본값 또는 에러 케이스
// }

// 명령어 노드 생성
t_command *create_command(t_shell *shell, t_token **tokens)
{
    t_command *cmd;
    t_token *curr;
    int arg_count;

    debug_print(2047, 7, "\n=== CREATE COMMAND ===\n");
    cmd = shell_malloc(shell, sizeof(t_command));
    if (!cmd)
        return (NULL);

    cmd->args = NULL;
    cmd->redirs = NULL;

    // 1단계: 먼저 명령어와 인자 개수를 세기
    curr = *tokens;
    arg_count = 0;
    while (curr && (curr->type == TOKEN_WORD || curr->type == TOKEN_REDIR))
    {
        if (curr->type == TOKEN_WORD)
            arg_count++;
        else if (curr->type == TOKEN_REDIR)
            curr = curr->next;  // 리다이렉션의 타겟 토큰 건너뛰기
        if (curr)
            curr = curr->next;
    }

    // 인자 배열 할당 및 초기화
    if (arg_count > 0)
    {
        cmd->args = shell_malloc(shell, sizeof(char *) * (arg_count + 1));
        if (!cmd->args)
            return (NULL);
        cmd->args[arg_count] = NULL;
    }

    // 2단계: 토큰을 다시 순회하며 명령어/인자와 리다이렉션 처리
    curr = *tokens;
    arg_count = 0;
    while (curr)
    {
        if (curr->type == TOKEN_WORD)
        {
            debug_print(2047, 7, "Processing word: %s\n", curr->value);
            cmd->args[arg_count++] = shell_strdup(shell, curr->value);
            curr = curr->next;
        }
        else if (curr->type == TOKEN_REDIR)
        {
            debug_print(2047, 7, "Processing redirection: %s\n", curr->value);
            t_token *next_token = curr->next;
            if (!next_token)
                return (NULL);

            t_redirection *redir = shell_malloc(shell, sizeof(t_redirection));
            if (!redir)
                return (NULL);

            redir->next = NULL;
            if (strcmp(curr->value, "<<") == 0)
            {
                redir->type = REDIR_HEREDOC;
                redir->filename = shell_strdup(shell, next_token->value);
            }
            else if (strcmp(curr->value, ">>") == 0)
            {
                redir->type = REDIR_APPEND;
                redir->filename = shell_strdup(shell, next_token->value);
            }
            else if (curr->value[0] == '<')
            {
                redir->type = REDIR_IN;
                redir->filename = shell_strdup(shell, next_token->value);
            }
            else
            {
                redir->type = REDIR_OUT;
                redir->filename = shell_strdup(shell, next_token->value);
            }

            // 리다이렉션 리스트에 추가
            if (!cmd->redirs)
                cmd->redirs = redir;
            else
            {
                t_redirection *last = cmd->redirs;
                while (last->next)
                    last = last->next;
                last->next = redir;
            }

            curr = next_token->next;
        }
        else
            break;
    }

    *tokens = curr;
    debug_print(2047, 7, "=== COMMAND CREATED ===\n");
    return cmd;
}

// 명령어 인자 배열 생성
char **create_args_array(t_shell *shell, t_token *start, int arg_count)
{
    char **args;
    int i;

    debug_print(2047, 3, "DEBUG: [create_args_array] Creating args array with count: %d\n", arg_count);
    args = shell_malloc(shell, sizeof(char *) * (arg_count + 1));
    if (!args)
    {
        debug_print(2047, 3, "DEBUG: [create_args_array] Failed to allocate args array\n");
        return (NULL);
    }

    i = 0;
    while (start && start->type == TOKEN_WORD && i < arg_count)
    {
        debug_print(2047, 3, "DEBUG: [create_args_array] Adding arg[%d]: %s\n", i, start->value);
        args[i] = shell_strdup(shell, start->value);
        if (!args[i])
        {
            debug_print(2047, 3, "DEBUG: [create_args_array] Failed to duplicate argument\n");
            return (NULL);
        }
        start = start->next;
        i++;
    }
    args[i] = NULL;
    debug_print(2047, 3, "DEBUG: [create_args_array] Args array created successfully\n");
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

    debug_print(2047, 3, "\nDEBUG: [parse_input] === Starting input parsing ===\n");

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
        debug_print(2047, 3, "DEBUG: [parse_input] Tokenization failed\n");
        return (1);
    }
    debug_print(2047, 3, "DEBUG: [parse_input] Tokenization completed\n");

    // 토큰 목록 출력
    debug_print(2047, 3, "\nDEBUG: [parse_input] Token list:\n");
    curr_token = shell->tokens;
    while (curr_token)
    {
        debug_print(2047, 3, "DEBUG: [parse_input] Token type: %d, value: %s\n",
               curr_token->type, curr_token->value);
        curr_token = curr_token->next;
    }

    curr_token = shell->tokens;
    shell->ast_root = parse_pipeline(shell, &curr_token);
    
    if (!shell->ast_root)
    {
        debug_print(2047, 3, "DEBUG: [parse_input] Pipeline parsing failed\n");
        return (1);
    }

    debug_print(2047, 3, "DEBUG: [parse_input] Parsing completed successfully\n");
    debug_print(2047, 3, "DEBUG: [parse_input] === Parsing finished ===\n\n");
    return (0);
}