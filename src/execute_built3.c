/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:53:23 by sanbaek           #+#    #+#             */
/*   Updated: 2025/03/19 09:59:51 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_show(t_env *current)
{
	while (current)
	{
		if (!current->key)
			current = current->next;
		else if (!current->value)
		{
			printf("declare -x ");
			printf("%s\n", current->key);
		}
		else if (current->value)
		{
			printf("declare -x ");
			printf("%s=\"%s\"\n", current->key, current->value);
		}
		current = current->next;
	}
}

t_env	*set_input_env(char *str, t_shell *shell)
{
	char	*equal_location;
	char	*stretch_location;
	t_env	*input_env;

	input_env = (t_env *)shell_malloc(shell, sizeof(t_env));
	equal_location = ft_strchr(str, '=');
	stretch_location = ft_strchr(str, '+');
	input_env->next = NULL;
	if (!equal_location)
	{
		input_env->value = NULL;
		input_env->key = shell_strdup(shell, str);
	}
	else
	{
		input_env->value = shell_strdup(shell, equal_location + 1);
		if (equal_location == str)
			input_env->key = shell_strdup(shell, "");
		else if (stretch_location && ((stretch_location + 1) == equal_location))
			input_env->key = ft_strndup(str, stretch_location - str, shell);
		else
			input_env->key = ft_strndup(str, equal_location - str, shell);
	}
	return (input_env);
}

char	*dup_val(char *value, t_shell *shell)
{
	size_t	len;
	char	*tmp;

	len = ft_strlen(value);
	tmp = shell_malloc(shell, len + 1);
	if (tmp)
		ft_memcpy(tmp, value, len + 1);
	return (tmp);
}

void	stretch_value(t_env *input_env, t_env *env_head, t_shell *shell)
{
	t_env	*target_env;

	target_env = find_already(input_env->key, env_head);
	if (!target_env)
	{
		add_env_value(shell, input_env->key, input_env->value);
		return ;
	}
	if (input_env->value[0] == '\0')
	{
		if (target_env->value)
			shell_free(shell, target_env->value);
		target_env->value = shell_strdup(shell, input_env->value);
		return ;
	}
	if (target_env->value)
		update_target_env_with_concat(target_env, input_env->value, shell);
	else
		target_env->value = shell_strdup(shell, input_env->value);
}

int	builtin_export(t_shell *shell, char **args)
{
	t_tree	*keyvalue_node;
	t_tree	node[100];

	convert_args_to_node(args, node);
	keyvalue_node = node->left_child;
	if (!keyvalue_node)
	{
		export_show(shell->env);
		return (0);
	}
	handle_export_values(keyvalue_node, shell->env, shell);
	return (1);
}
