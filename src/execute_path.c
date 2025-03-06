/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 05:04:13 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 04:21:02 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_value(t_shell *shell)
{
	t_env	*current;

	if (!shell)
		return (NULL);
	if (!shell->env)
		return (NULL);
	current = shell->env;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, "PATH") == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

int	count_path_segments(char *path_str)
{
	int	count;
	int	i;

	count = 1;
	i = 0;
	while (path_str[i])
	{
		if (path_str[i] == ':')
			count++;
		i++;
	}
	return (count);
}

int	extract_path_seg(t_shell *shell, char **paths, char *path_str, int count)
{
	int		i;
	char	*start;
	char	*end;

	i = 0;
	start = path_str;
	while (i < count)
	{
		end = ft_strchr(start, ':');
		if (!end)
			end = start + ft_strlen(start);
		paths[i] = shell_malloc(shell, end - start + 1);
		if (!paths[i])
			return (0);
		ft_strncpy(paths[i], start, end - start);
		paths[i][end - start] = '\0';
		start = end + 1;
		i++;
	}
	paths[i] = NULL;
	return (1);
}

char	**split_path(t_shell *shell, char *path_str)
{
	char	**paths;
	int		count;

	count = count_path_segments(path_str);
	paths = shell_malloc(shell, sizeof(char *) * (count + 1));
	if (!paths)
		return (NULL);
	if (!extract_path_seg(shell, paths, path_str, count))
		return (NULL);
	return (paths);
}
