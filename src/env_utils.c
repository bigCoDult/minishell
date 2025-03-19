/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:27:33 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/19 10:07:38 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_env_arg(t_shell *shell, char *arg, char **key, char **value)
{
	char	*equals;

	equals = ft_strchr(arg, '=');
	if (!equals)
	{
		*key = shell_strdup(shell, arg);
		*value = NULL;
		return (0);
	}
	*equals = '\0';
	*key = shell_strdup(shell, arg);
	*value = shell_strdup(shell, equals + 1);
	*equals = '=';
	if (!*key || (*value && !**value))
		return (1);
	return (0);
}

void	add_env_value(t_shell *shell, const char *key, const char *value)
{
	t_env	*new_node;

	new_node = shell_malloc(shell, sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = shell_strdup(shell, key);
	if (value)
		new_node->value = shell_strdup(shell, value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (!new_node->key)
	{
		shell_free(shell, new_node);
		return ;
	}
	new_node->next = shell->env;
	shell->env = new_node;
}

void	set_env_value(t_shell *shell, const char *key, const char *value)
{
	t_env	*current;

	if (!key || !*key)
		return ;
	current = shell->env;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, key) == 0)
		{
			if (current->value)
			{
				shell_free(shell, current->value);
				current->value = NULL;
			}
			if (value)
				current->value = shell_strdup(shell, value);
			return ;
		}
		current = current->next;
	}
	add_env_value(shell, key, value);
}

t_env	*create_env_node(t_shell *shell, char *key, char *value)
{
	t_env	*new_node;

	new_node = shell_malloc(shell, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = shell_strdup(shell, key);
	new_node->value = shell_strdup(shell, value);
	new_node->next = NULL;
	return (new_node);
}

void	add_env_node(t_env **env_list, t_env *new_node)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
