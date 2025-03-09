/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:02:59 by sanbaek           #+#    #+#             */
/*   Updated: 2025/03/09 21:03:03 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// char	*ft_strndup(const char *s, size_t n)
// {
// 	char	*dup;
// 	size_t	len;
// 	size_t	i;

// 	len = 0;
// 	while (len < n && s[len])
// 		len++;
// 	dup = (char *)ft_malloc(len + 1);
// 	if (!dup)
// 		return (NULL);
// 	i = 0;
// 	while (i < len)
// 	{
// 		dup[i] = s[i];
// 		i++;
// 	}
// 	dup[i] = '\0';
// 	return (dup);
// }

// char	*ft_strnstr(const char *big, const char *little, size_t len)
// {
// 	size_t	i;
// 	size_t	k;

// 	if (*little == '\0')
// 		return ((char *)big);
// 	i = 0;
// 	while (i < len && big[i])
// 	{
// 		k = 0;
// 		while (big[i + k] == little[k] && i + k < len)
// 		{
// 			if (little[++k] == '\0')
// 				return ((char *)(big + i));
// 		}
// 		i++;
// 	}
// 	return (NULL);
// }







// void	add_keyempty(char *key, t_env *current)
// {
// 	t_env	*new_node;

// 	if (find_already(key, current))
// 		return ;
// 	new_node = create_env_node(key, NULL);
// 	if (!new_node)
// 		return ;
// 	while (current->next)
// 		current = current->next;
// 	current->next = new_node;
// }

// void	stretch_value(t_env *input_env, t_env *current)
// {
// 	t_env	*target_env;

// 	target_env = find_already(input_env->key, current);
// 	if (!target_env)
// 	{
// 		make_tail(input_env, current);
// 		return ;
// 	}
// 	if (target_env->value)
// 		target_env->value = ft_strjoin(target_env->value, input_env->value);
// 	else
// 		target_env->value = ft_strdup(input_env->value);
// }




// void	export_show(t_env *current)
// {
// 	while (current)
// 	{
// 		if (!current->key)
// 			current = current->next;
// 		else if (!current->value)
// 		{
// 			printf("declare -x ");
// 			printf("%s\n", current->key);
// 		}
// 		else if (current->value)
// 		{
// 			printf("declare -x ");
// 			printf("%s=%s\n", current->key, current->value);
// 		}
// 		current = current->next;
// 	}
// }

// t_env	*set_input_env(char *str)
// {
// 	char	*equal_location;
// 	char	*stretch_location;
// 	t_env	*input_env;

// 	input_env = (t_env *)ft_malloc(sizeof(t_env));
// 	equal_location = ft_strchr(str, '=');
// 	stretch_location = ft_strchr(str, '+');
// 	input_env->next = NULL;
// 	if (!equal_location)
// 		input_env->value = NULL;
// 	else
// 		input_env->value = ft_strdup(equal_location + 1);
// 	if (stretch_location && ((stretch_location + 1) == equal_location))
// 		input_env->key = ft_strndup(str, stretch_location - str);
// 	else
// 		input_env->key = ft_strndup(str, equal_location - str);
// 	return (input_env);
// }

// static void	process_export_value(t_env *input_env, t_env *env_head, char *str)
// {
// 	t_env	*current;

// 	current = env_head;
// 	if (!input_env->value)
// 		add_keyempty(str, current);
// 	else if (ft_strnstr(str, "+=", ft_strlen(str)))
// 		stretch_value(input_env, current);
// 	else
// 		add_keyvalue(input_env, current);
// }

// static void	handle_export_values(t_tree *keyvalue_node, t_env *env_head)
// {
// 	t_env	*input_env;
// 	char	*str;

// 	while (keyvalue_node)
// 	{
// 		str = keyvalue_node->token->value;
// 		input_env = set_input_env(str);
// 		if (!is_valid_identifier(input_env->key))
// 		{
// 			keyvalue_node = keyvalue_node->right_sibling;
// 			continue ;
// 		}
// 		process_export_value(input_env, env_head, str);
// 		keyvalue_node = keyvalue_node->right_sibling;
// 	}
// }

// void	handle_export_command(t_tree *cmd_node, t_env *env_head)
// {
// 	t_tree	*keyvalue_node;

// 	if (!cmd_node)
// 		return ;
// 	keyvalue_node = cmd_node->left_child;
// 	if (!keyvalue_node)
// 	{
// 		export_show(env_head);
// 		return ;
// 	}
// 	handle_export_values(keyvalue_node, env_head);
// }



int	builtin_export(t_shell *shell, char **args)
{
	char	*key;
	char	*value;
	int		i;
	
	i = 1;
	while (args[i])
	{
		if (parse_env_arg(shell, args[i], &key, &value))
		{
			printf("export: '%s': not a valid identifier\n", args[i]);
			return (1);
		}
		set_env_value(shell, key, value);
		shell_free(shell, key);
		if (value)
		shell_free(shell, value);
		i++;
	}
	return (0);
}