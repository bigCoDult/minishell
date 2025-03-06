/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 05:07:15 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/05 13:33:04 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env_array(t_shell *shell)
{
	char	**env_array;
	t_env	*current;
	int		count;

	count = 0;
	current = shell->env;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = shell_malloc(shell, sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = shell->env;
	count = 0;
	while (current)
	{
		env_array[count] = shell_malloc(shell, ft_strlen(current->key)
				+ ft_strlen(current->value) + 2);
		if (!env_array[count])
			return (NULL);
		sprintf(env_array[count], "%s=%s", current->key, current->value);
		count++;
		current = current->next;
	}
	env_array[count] = NULL;
	return (env_array);
}
