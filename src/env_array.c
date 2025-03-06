/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 05:07:15 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 08:45:13 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**allocate_env_array(t_shell *shell, int *count)
{
	char	**env_array;
	t_env	*current;

	*count = 0;
	current = shell->env;
	while (current)
	{
		(*count)++;
		current = current->next;
	}
	env_array = shell_malloc(shell, sizeof(char *) * (*count + 1));
	return (env_array);
}

char	**get_env_array(t_shell *shell)
{
	char	**env_array;
	t_env	*current;
	int		count;
	int		i;

	env_array = allocate_env_array(shell, &count);
	if (!env_array)
		return (NULL);
	current = shell->env;
	i = 0;
	while (current)
	{
		env_array[i] = shell_malloc(shell, ft_strlen(current->key)
				+ ft_strlen(current->value) + 2);
		if (!env_array[i])
			return (NULL);
		// sprintf?
		sprintf(env_array[i], "%s=%s", current->key, current->value);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
