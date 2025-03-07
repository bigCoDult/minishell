/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:29:38 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 18:54:34 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_variable(t_shell *shell, char **write_pos, const char **str)
{
	char	*var_name;
	char	*var_value;

	(*str)++;
	if (**str == '?')
	{
		expand_exit_code(shell, write_pos, str);
		return ;
	}
	var_name = extract_var_name(shell, str);
	var_value = get_env_value(shell, var_name);
	if (var_value)
	{
		ft_strcpy(*write_pos, var_value);
		*write_pos += ft_strlen(var_value);
	}
	shell_free(shell, var_name);
}

int	should_expand_var(const char *str, int in_single_quote)
{
	return (*str == '$' && !in_single_quote
		&& (ft_isalnum(*(str + 1)) || *(str + 1) == '_' || *(str + 1) == '?'));
}

char	*expand_env_var(t_shell *shell, const char *str)
{
	char	*result;
	char	*write_pos;
	int		in_single_quote;

	if (!str)
		return (NULL);
	in_single_quote = 0;
	result = shell_malloc(shell, strlen(str) * 200000);
	write_pos = result;
	while (*str)
	{
		handle_single_quote(&str, &in_single_quote);
		if (*str && should_expand_var(str, in_single_quote))
			expand_variable(shell, &write_pos, &str);
		else if (*str)
			copy_char(&write_pos, &str);
	}
	*write_pos = '\0';
	return (result);
}

void	expand_exit_code(t_shell *shell, char **write_pos, const char **str)
{
	char	status[16];

	(*str)++;
	ft_itoa_n(status, sizeof(status), shell->status.exit_code);
	ft_strcpy(*write_pos, status);
	*write_pos += ft_strlen(status);
}
