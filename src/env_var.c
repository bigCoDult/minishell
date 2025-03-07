/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:32:18 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 18:54:47 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_var_name(t_shell *shell, const char **str)
{
	const char	*var_start;
	int			var_len;
	char		*var_name;

	var_start = *str;
	while (ft_isalnum(**str) || **str == '_')
		(*str)++;
	var_len = *str - var_start;
	var_name = shell_malloc(shell, var_len + 1);
	ft_strncpy(var_name, var_start, var_len);
	var_name[var_len] = '\0';
	return (var_name);
}

char	*handle_special_var(t_shell *shell, const char *var_name)
{
	char	status[16];

	if (ft_strcmp(var_name, "?") == 0)
	{
		ft_itoa_n(status, sizeof(status), shell->status.exit_code);
		return (shell_strdup(shell, status));
	}
	return (NULL);
}
