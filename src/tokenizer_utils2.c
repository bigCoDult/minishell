/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:58:23 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 08:32:23 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*finalize_word(t_shell *shell, char *word, int quote_state)
{
	char	*result;
//ft_strcnmp
	if (quote_state != 1 && strncmp(word, "$\"", 2) != 0)
		result = expand_env_var(shell, word);
	else
		result = shell_strdup(shell, word);
	shell_free(shell, word);
	return (result);
}

char	*handle_word(t_shell *shell, char *input, int *len)
{
	char	*word;
	char	*result;
	int		quote_state;
	int		start_idx;

	*len = get_word_length(input);
	if (*len == 0)
		return (NULL);
	word = shell_malloc(shell, *len + 1);
	if (!word)
		return (NULL);
	quote_state = determine_quote_state(input, &start_idx);
	process_word_content(input, word, *len, start_idx);
	result = finalize_word(shell, word, quote_state);
	return (result);
}
