/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:53:23 by sanbaek           #+#    #+#             */
/*   Updated: 2025/03/13 03:06:40 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define UPPER_ALPHA_RANGE_START	65
#define UPPER_ALPHA_RANGE_END		90
#define LOWER_ALPHA_RANGE_START	97
#define LOWER_ALPHA_RANGE_END		122

static int	ft_is_range(int c, int start, int end)
{
	return (c >= start && c <= end);
}

static int	ft_is_loweralpha(int c)
{
	return (ft_is_range(c, LOWER_ALPHA_RANGE_START, LOWER_ALPHA_RANGE_END));
}

static int	ft_is_upperalpha(int c)
{
	return (ft_is_range(c, UPPER_ALPHA_RANGE_START, UPPER_ALPHA_RANGE_END));
}

int	ft_isalpha(int c)
{
	return (ft_is_loweralpha(c) || ft_is_upperalpha(c));
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 1;
	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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
		input_env->value = NULL;
	else
		input_env->value = shell_strdup(shell, equal_location + 1);
	if (stretch_location && ((stretch_location + 1) == equal_location))
		input_env->key = ft_strndup(str, stretch_location - str, shell);
	else
		input_env->key = ft_strndup(str, equal_location - str, shell);
	return (input_env);
}

static char	*dup_val(char *value, t_shell *shell)
{
	size_t	len;
	char	*tmp;

	len = ft_strlen(value);
	tmp = shell_malloc(shell, len + 1);
	if (tmp)
		ft_memcpy(tmp, value, len + 1);
	return (tmp);
}

static void	update_target_env_with_concat(t_env *target_env, char *new_part, t_shell *shell)
{
	size_t	len1;
	size_t	len2;
	char	*new_value;

	len1 = ft_strlen(target_env->value);
	len2 = ft_strlen(new_part);
	new_value = shell_malloc(shell, len1 + len2 + 1);
	if (new_value)
	{
		ft_memcpy(new_value, target_env->value, len1);
		ft_memcpy(new_value + len1, new_part, len2 + 1);
	}
	shell_free(shell, target_env->value);
	target_env->value = new_value;
}

void	stretch_value(t_env *input_env, t_env *env_head, t_shell *shell)
{
	t_env	*target_env;

	target_env = find_already(input_env->key, env_head);
	if (!target_env)
	{
		add_env_value(shell, input_env->key, dup_val(input_env->value, shell));
		return ;
	}
	if (input_env->value[0] == '\0')
	{
		if (target_env->value)
			shell_free(shell, target_env->value);
		target_env->value = dup_val(input_env->value, shell);
		return ;
	}
	if (target_env->value)
		update_target_env_with_concat(target_env, input_env->value, shell);
	else
		target_env->value = dup_val(input_env->value, shell);
}

static void	process_export_value(t_env *input_env, t_env *env_head, char *str,
		t_shell *shell)
{
	t_env	*target_env;

	target_env = find_already(input_env->key, env_head);
	if (input_env->value == NULL)
	{
		if (!target_env)
			add_env_value(shell, input_env->key, NULL);
	}
	else if (ft_strnstr(str, "+=", ft_strlen(str)))
		stretch_value(input_env, env_head, shell);
	else
	{
		if (target_env)
		{
			if (target_env->value)
				shell_free(shell, target_env->value);
			target_env->value = dup_val(input_env->value, shell);
		}
		else
			add_env_value(shell, input_env->key, dup_val(input_env->value, shell));
	}
}

static void	handle_export_values(t_tree *keyvalue_node, t_env *env_head,
		t_shell *shell)
{
	t_env	*input_env;
	char	*str;

	while (keyvalue_node)
	{
		str = keyvalue_node->value;
		input_env = set_input_env(str, shell);
		if (!is_valid_identifier(input_env->key))
		{
			shell_free(shell, input_env->key);
			if (input_env->value)
				shell_free(shell, input_env->value);
			shell_free(shell, input_env);
			keyvalue_node = keyvalue_node->right_sibling;
			continue ;
		}
		process_export_value(input_env, env_head, str, shell);
		shell_free(shell, input_env->key);
		if (input_env->value)
			shell_free(shell, input_env->value);
		shell_free(shell, input_env);
		keyvalue_node = keyvalue_node->right_sibling;
	}
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
