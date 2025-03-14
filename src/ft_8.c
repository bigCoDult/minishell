/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_8.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 07:45:08 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/14 14:57:04 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalpha(int c)
{
	if (c >= 65 && c <= 90)
		return (1);
	else if (c >= 97 && c <= 122)
		return (1);
	return (0);
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

void	update_target_env_with_concat(t_env *target_env,
	char *new_part, t_shell *shell)
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

void	process_export_value(t_env *input_env, t_env *env_head, char *str,
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
			add_env_value(shell, input_env->key,
				dup_val(input_env->value, shell));
	}
}

void	handle_export_values(t_tree *keyvalue_node, t_env *env_head,
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
			print_error("minishell: export: `%s': not valid identifier\n", str);
			shell->status.exit_code = 1;
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
