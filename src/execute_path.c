/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 05:04:13 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/12 13:27:59 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// PATH 환경변수 값 가져오기
static char *get_path_value(t_shell *shell)
{
    t_env *current;

    debug_print(2047, 6, "DEBUG: [get_path_value] Starting to get PATH value\n");
    
    if (!shell)
    {
        debug_print(2047, 6, "DEBUG: [get_path_value] Shell is NULL\n");
        return (NULL);
    }

    if (!shell->env)
    {
        debug_print(2047, 6, "DEBUG: [get_path_value] Shell env is NULL\n");
        return (NULL);
    }

    debug_print(2047, 6, "DEBUG: [get_path_value] Searching through environment variables\n");
    current = shell->env;
    
    while (current)
    {
        debug_print(2047, 6, "DEBUG: [get_path_value] Checking env: %s=%s\n",
               current->key ? current->key : "NULL", 
               current->value ? current->value : "NULL");
        
        if (current->key && strcmp(current->key, "PATH") == 0)
        {
            debug_print(2047, 6, "DEBUG: [get_path_value] Found PATH: %s\n", current->value);
            return (current->value);
        }
        current = current->next;
    }
    
    debug_print(2047, 6, "DEBUG: [get_path_value] PATH not found in environment\n");
    return (NULL);
}

// 문자열을 ':' 기준으로 분리
static char **split_path(t_shell *shell, char *path_str)
{
    char **paths;
    int count;
    int i;
    char *start;
    char *end;

    // 경로 개수 세기
    count = 1;
    i = 0;
    while (path_str[i])
    {
        if (path_str[i] == ':')
            count++;
        i++;
    }

    // 경로 배열 할당
    paths = shell_malloc(shell, sizeof(char *) * (count + 1));
    if (!paths)
        return (NULL);

    // 경로 분리
    i = 0;
    start = path_str;
    while (i < count)
    {
        end = strchr(start, ':');
        if (!end)
            end = start + strlen(start);

        paths[i] = shell_malloc(shell, end - start + 1);
        if (!paths[i])
            return (NULL);
        strncpy(paths[i], start, end - start);
        paths[i][end - start] = '\0';

        start = end + 1;
        i++;
    }
    paths[i] = NULL;
    return (paths);
}

// 주어진 경로에서 실행 가능한 파일인지 확인
static int is_executable(const char *path)
{
    struct stat st;

    if (stat(path, &st) == 0)
    {
        if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
            return (1);
    }
    return (0);
}

// PATH에서 실행 파일 찾기
char *find_executable(t_shell *shell, const char *cmd)
{
    char *path;
    char *dir;
    char *full_path;
    char *path_copy;

    debug_print(2047, 6, "DEBUG: [find_executable] Starting search for: %s\n", cmd);
    
    // 명령어가 절대 경로나 상대 경로인 경우
    if (cmd[0] == '/' || cmd[0] == '.')
    {
        debug_print(2047, 6, "DEBUG: [find_executable] Command is a path\n");
        return (shell_strdup(shell, cmd));
    }

    // PATH 환경 변수 가져오기
    debug_print(2047, 6, "DEBUG: [find_executable] Getting PATH value\n");
    path = get_path_value(shell);
    if (!path)
    {
        debug_print(2047, 6, "DEBUG: [find_executable] PATH not found\n");
        return (NULL);
    }

    debug_print(2047, 6, "DEBUG: [find_executable] PATH value: %s\n", path);
    path_copy = shell_strdup(shell, path);
    if (!path_copy)
    {
        debug_print(2047, 6, "DEBUG: [find_executable] Failed to copy PATH\n");
        return (NULL);
    }

    debug_print(2047, 6, "DEBUG: [find_executable] Searching in PATH directories\n");
    dir = strtok(path_copy, ":");
    
    while (dir)
    {
        debug_print(2047, 6, "DEBUG: [find_executable] Checking directory: %s\n", dir);
        full_path = shell_malloc(shell, strlen(dir) + strlen(cmd) + 2);
        if (!full_path)
        {
            debug_print(2047, 6, "DEBUG: [find_executable] Memory allocation failed\n");
            shell_free(shell, path_copy);
            return (NULL);
        }
        
        sprintf(full_path, "%s/%s", dir, cmd);
        debug_print(2047, 6, "DEBUG: [find_executable] Trying path: %s\n", full_path);
        
        if (access(full_path, X_OK) == 0)
        {
            debug_print(2047, 6, "DEBUG: [find_executable] Executable found at: %s\n", full_path);
            shell_free(shell, path_copy);
            return (full_path);
        }
        
        debug_print(2047, 6, "DEBUG: [find_executable] Not found in this directory\n");
        shell_free(shell, full_path);
        dir = strtok(NULL, ":");
    }
    
    debug_print(2047, 6, "DEBUG: [find_executable] Executable not found in PATH\n");
    shell_free(shell, path_copy);
    return (NULL);
}

// 명령어의 전체 경로 찾기
char *find_command_path(t_shell *shell, const char *cmd)
{
    char **paths;
    char *path_str;
    char *full_path;
    int i;

    debug_print(2047, 6, "DEBUG: find_command_path start for cmd: %s\n", cmd);

    // 명령어가 이미 전체 경로인 경우
    if (cmd[0] == '/' || cmd[0] == '.')
    {
        debug_print(2047, 6, "DEBUG: Command is a path\n");
        if (is_executable(cmd))
            return (shell_strdup(shell, cmd));
        return (NULL);
    }

    // PATH 환경변수 가져오기
    path_str = get_path_value(shell);
    debug_print(2047, 6, "DEBUG: PATH value: %s\n", path_str ? path_str : "NULL");
    if (!path_str)
        return (NULL);

    // PATH를 개별 경로로 분리
    paths = split_path(shell, path_str);
    if (!paths)
    {
        debug_print(2047, 6, "DEBUG: Failed to split PATH\n");
        return (NULL);
    }

    // 각 경로에서 명령어 찾기
    i = 0;
    while (paths[i])
    {
        debug_print(2047, 6, "DEBUG: Checking path: %s\n", paths[i]);
        full_path = shell_malloc(shell, strlen(paths[i]) + strlen(cmd) + 2);
        if (!full_path)
        {
            debug_print(2047, 6, "DEBUG: Memory allocation failed\n");
            return (NULL);
        }

        sprintf(full_path, "%s/%s", paths[i], cmd);
        debug_print(2047, 6, "DEBUG: Checking full path: %s\n", full_path);
        
        if (is_executable(full_path))
        {
            debug_print(2047, 6, "DEBUG: Found executable at: %s\n", full_path);
            return (full_path);
        }
        
        shell_free(shell, full_path);
        i++;
    }

    debug_print(2047, 6, "DEBUG: Command not found in any PATH\n");
    return (NULL);
}