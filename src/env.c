/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:33:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_env_arg(t_shell *shell, char *arg, char **key, char **value)
{
	char	*equals;

	debug_print(2047, 2, "DEBUG: [parse_env_arg] Parsing: %s\n", arg);
	equals = strchr(arg, '=');
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
	debug_print(2047, 2, "DEBUG: [parse_env_arg] Parsed key: %s, value: %s\n", *key, *value ? *value : "(null)");
	if (!*key || (*value && !**value))
		return (1);
	return (0);
}

void	set_env_value(t_shell *shell, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;

	debug_print(2047, 2, "DEBUG: [set_env_value] Attempting to set %s=%s\n",
		   key, value ? value : "(null)");
	if (!key || !*key)
		return ;
	current = shell->env;
	while (current)
	{
		if (current->key && strcmp(current->key, key) == 0)
		{
			debug_print(2047, 2, "DEBUG: [set_env_value] Found existing key: %s\n", key);
			if (current->value)
			{
				free(current->value);
				current->value = NULL;
			}
			if (value)
			{
				current->value = ft_strdup(value);
				debug_print(2047, 2, "DEBUG: [set_env_value] Updated value: %s\n", current->value);
			}
			return ;
		}
		current = current->next;
	}
	debug_print(2047, 2, "DEBUG: [set_env_value] Creating new env variable\n");
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		debug_print(2047, 2, "DEBUG: [set_env_value] Failed to allocate new node\n");
		return ;
	}
	new_node->key = ft_strdup(key);
	new_node->value = value ? ft_strdup(value) : NULL;
	new_node->next = NULL;
	if (!new_node->key)
	{
		debug_print(2047, 2, "DEBUG: [set_env_value] Failed to allocate key\n");
		free(new_node);
		return ;
	}
	debug_print(2047, 2, "DEBUG: [set_env_value] Adding to env list\n");
	new_node->next = shell->env;
	shell->env = new_node;
	debug_print(2047, 2, "DEBUG: [set_env_value] Successfully added %s=%s\n",
		   new_node->key, new_node->value ? new_node->value : "(null)");
}

static t_env	*create_env_node(t_shell *shell, char *key, char *value)
{
	t_env	*new_node;

	(void)shell;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

static void	add_env_node(t_env **env_list, t_env *new_node)
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

t_env	*init_env(t_shell *shell, char **envp)
{
	t_env	*env_list;
	char	*equal_sign;
	char	*key;
	char	*value;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			key = envp[i];
			value = equal_sign + 1;
			add_env_node(&env_list, create_env_node(shell, key, value));
			*equal_sign = '=';
		}
		i++;
	}
	return (env_list);
}

char	*get_env_value(t_shell *shell, const char *key)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return current->value;
		current = current->next;
	}
	return (NULL);
}

char	*expand_env_var(t_shell *shell, const char *str)
{
	char		*result;
	char		*write_pos;
	int			in_single_quote;
	char		status[16];
	char		*var_name;
	char		*var_value;
	int			var_len;
	const char	*var_start;

	in_single_quote = 0;
	if (!str)
		return (NULL);
	result = shell_malloc(shell, strlen(str) * 2);
	write_pos = result;
	while (*str)
	{
		if (*str == '\'')
		{
			in_single_quote = !in_single_quote;
			str++;
			continue ;
		}
		if (*str == '$' && !in_single_quote
			&& (isalnum(*(str + 1)) || *(str + 1) == '_' || *(str + 1) == '?'))
		{
			str++;
			if (*str == '?')
			{
				snprintf(status, sizeof(status), "%d", shell->status.exit_code);
				strcpy(write_pos, status);
				write_pos += strlen(status);
				str++;
				continue ;
			}
			var_start = str;
			while (isalnum(*str) || *str == '_')
				str++;
			var_len = str - var_start;
			var_name = shell_malloc(shell, var_len + 1);
			strncpy(var_name, var_start, var_len);
			var_name[var_len] = '\0';
			var_value = get_env_value(shell, var_name);
			if (var_value)
			{
				strcpy(write_pos, var_value);
				write_pos += strlen(var_value);
			}
			shell_free(shell, var_name);
		}
		else
			*write_pos++ = *str++;
	}
	*write_pos = '\0';
	return (result);
}

char	*handle_special_var(t_shell *shell, const char *var_name)
{
	char	status[16];

	if (strcmp(var_name, "?") == 0)
	{
		snprintf(status, sizeof(status), "%d", shell->status.exit_code);
		return (shell_strdup(shell, status));
	}
	return (NULL);
}