/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:33:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/05 13:32:43 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	handle_single_quote(const char **str, int *in_single_quote)
{
	if (**str == '\'')
	{
		*in_single_quote = !(*in_single_quote);
		(*str)++;
	}
}

void	copy_char(char **write_pos, const char **str)
{
	*(*write_pos)++ = *(*str)++;
}
