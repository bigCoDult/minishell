/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:11:23 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 06:15:56 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_keyvalue(t_env *input_env, t_env *env_head)
{
	t_env	*current;

	if (!input_env || !env_head)
		return ;
	if (!(input_env->key) || !(input_env->value))
		return ;
	current = env_head;
	while (current)
	{
		if (current->key && (ft_strcmp(current->key, input_env->key) == 0))
		{
			current->value = ft_strdup(input_env->value);
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	add_env_node(&env_head, input_env);
}

char	*get_env(t_env *head, char *key)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env	*find_already(char *key, t_env *env_head)
{
	t_env	*current;

	current = env_head;
	while (current)
	{
		if (current->key && (ft_strcmp(current->key, key) == 0))
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	export_for_cd(char *key, char *value, t_env **env_head, t_shell *shell)
{
	if (!find_already(key, *env_head))
		add_env_value(shell, key, value);
	else
		set_env_value(shell, key, value);
}
