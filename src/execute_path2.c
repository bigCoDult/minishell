/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:19:30 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/07 03:07:05 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_executable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
			return (1);
	}
	return (0);
}

char	*search_in_path(t_shell *shell, const char *cmd, char *path_copy)
{
	char	*dir;
	char	*full_path;

	dir = ft_strtok(path_copy, ":");
	while (dir)
	{
		full_path = shell_malloc(shell, ft_strlen(dir)
				+ ft_strlen(cmd) + 2);
		if (!full_path)
			return (NULL);
		ft_strcpy(full_path, dir);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		shell_free(shell, full_path);
		dir = ft_strtok(NULL, ":");
	}
	return (NULL);
}

char	*find_executable(t_shell *shell, const char *cmd)
{
	char	*path;
	char	*path_copy;
	char	*result;

	if (cmd[0] == '/' || cmd[0] == '.')
		return (shell_strdup(shell, cmd));
	path = get_path_value(shell);
	if (!path)
		return (NULL);
	path_copy = shell_strdup(shell, path);
	if (!path_copy)
		return (NULL);
	result = search_in_path(shell, cmd, path_copy);
	shell_free(shell, path_copy);
	return (result);
}

char	*search_command_in_paths(t_shell *shell, const char *cmd, char **paths)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = shell_malloc(shell, ft_strlen(paths[i])
				+ ft_strlen(cmd) + 2);
		if (!full_path)
			return (NULL);
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (is_executable(full_path))
			return (full_path);
		shell_free(shell, full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(t_shell *shell, const char *cmd)
{
	char	**paths;
	char	*path_str;
	char	*result;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (is_executable(cmd))
			return (shell_strdup(shell, cmd));
		return (NULL);
	}
	path_str = get_path_value(shell);
	if (!path_str)
		return (NULL);
	paths = split_path(shell, path_str);
	if (!paths)
		return (NULL);
	result = search_command_in_paths(shell, cmd, paths);
	return (result);
}
