/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 05:04:13 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 05:13:52 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// PATH 환경변수 값 가져오기
static char *get_path_value(t_shell *shell)
{
    t_env *current;

    current = shell->env;
    while (current)
    {
        if (strcmp(current->key, "PATH") == 0)
            return (current->value);
        current = current->next;
    }
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

// 명령어의 전체 경로 찾기
char *find_command_path(t_shell *shell, const char *cmd)
{
    char **paths;
    char *path_str;
    char *full_path;
    int i;

    printf("DEBUG: find_command_path start for cmd: %s\n", cmd);

    // 명령어가 이미 전체 경로인 경우
    if (cmd[0] == '/' || cmd[0] == '.')
    {
        printf("DEBUG: Command is a path\n");
        if (is_executable(cmd))
            return (shell_strdup(shell, cmd));
        return (NULL);
    }

    // PATH 환경변수 가져오기
    path_str = get_path_value(shell);
    printf("DEBUG: PATH value: %s\n", path_str ? path_str : "NULL");
    if (!path_str)
        return (NULL);

    // PATH를 개별 경로로 분리
    paths = split_path(shell, path_str);
    if (!paths)
    {
        printf("DEBUG: Failed to split PATH\n");
        return (NULL);
    }

    // 각 경로에서 명령어 찾기
    i = 0;
    while (paths[i])
    {
        printf("DEBUG: Checking path: %s\n", paths[i]);
        full_path = shell_malloc(shell, strlen(paths[i]) + strlen(cmd) + 2);
        if (!full_path)
        {
            printf("DEBUG: Memory allocation failed\n");
            return (NULL);
        }

        sprintf(full_path, "%s/%s", paths[i], cmd);
        printf("DEBUG: Checking full path: %s\n", full_path);
        
        if (is_executable(full_path))
        {
            printf("DEBUG: Found executable at: %s\n", full_path);
            return (full_path);
        }
        
        shell_free(shell, full_path);
        i++;
    }

    printf("DEBUG: Command not found in any PATH\n");
    return (NULL);
}