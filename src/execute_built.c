/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:42:51 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo 명령어: 인자들을 출력
int builtin_echo(char **args)
{
    int i;
    int n_option;

    n_option = 0;
    i = 1;
    
    // -n 옵션 체크
    if (args[1] && strcmp(args[1], "-n") == 0)
    {
        n_option = 1;
        i++;
    }
    
    // 인자들 출력
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    
    if (!n_option)
        printf("\n");
    return (0);
}

// cd 명령어: 디렉토리 변경
int builtin_cd(t_shell *shell, char **args)
{
    char *path;
    char current_path[1024];

    // 인자가 없으면 HOME으로
    if (!args[1])
        path = getenv("HOME");
    else
        path = args[1];
    
    if (chdir(path) != 0)
    {
        printf("cd: %s: No such file or directory\n", path);
        return (1);
    }
    
    // PWD 환경변수 업데이트
    if (getcwd(current_path, sizeof(current_path)))
        set_env_value(shell, "PWD", current_path);
    
    return (0);
}

// pwd 명령어: 현재 작업 디렉토리 출력
int builtin_pwd(void)
{
    char current_path[1024];

    if (getcwd(current_path, sizeof(current_path)))
    {
        printf("%s\n", current_path);
        return (0);
    }
    return (1);
}

// export 명령어: 환경변수 설정
int builtin_export(t_shell *shell, char **args)
{
    // char *equal_sign;
    char *key;
    char *value;
    int i;

    i = 1;
    while (args[i])
    {
        debug_print(0, 2, "DEBUG: [builtin_export] Processing arg: %s\n", args[i]);
        if (parse_env_arg(shell, args[i], &key, &value))
        {
            printf("export: '%s': not a valid identifier\n", args[i]);
            return (1);
        }
        set_env_value(shell, key, value);
        shell_free(shell, key);
        if (value)
            shell_free(shell, value);
        i++;
    }
    return (0);
}

// unset 명령어: 환경변수 삭제
int builtin_unset(t_shell *shell, char **args)
{
    int i;

    i = 1;
    while (args[i])
    {
        // 환경변수 리스트에서 해당 키를 찾아 삭제
        t_env *current = shell->env;
        t_env *prev = NULL;

        while (current)
        {
            if (strcmp(current->key, args[i]) == 0)
            {
                if (prev)
                    prev->next = current->next;
                else
                    shell->env = current->next;
                shell_free(shell, current->key);
                shell_free(shell, current->value);
                shell_free(shell, current);
                break;
            }
            prev = current;
            current = current->next;
        }
        i++;
    }
    return (0);
}

// env 명령어: 환경변수 목록 출력
int builtin_env(t_shell *shell)
{
    t_env *current;

    debug_print(0, 2, "DEBUG: [builtin_env] Starting to print environment\n");

    current = shell->env;
    while (current)
    {
        if (current->value)  // value가 있는 환경변수만 출력
        {
            debug_print(0, 2, "DEBUG: [builtin_env] Printing: %s=%s\n",
                   current->key, current->value);
            printf("%s=%s\n", current->key, current->value);
        }
        current = current->next;
    }

    return (0);
}

// exit 명령어: 셸 종료
int builtin_exit(t_shell *shell, char **args)
{
    int exit_code;

    if (args[1])
    {
        // 숫자로 변환 가능한지 체크 필요
        exit_code = atoi(args[1]);
    }
    else
        exit_code = shell->status.exit_code;

    printf("exit\n");
    free_all_memory(shell);
	free_env(shell);
    exit(exit_code);
}