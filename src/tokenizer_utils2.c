/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:58:23 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/12 13:10:06 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*finalize_word(t_shell *shell, char *word, int quote_state)
{
	char	*result;
	char	*temp;
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;

	(void)quote_state;
	temp = shell_malloc(shell, strlen(word) * 2);
	if (!temp)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;

	while (word[i])
	{
		if (word[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
		}
		else if (word[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++;
		}
		else if ((word[i] == '$') && !in_single_quote && 
				(ft_isalnum(word[i + 1]) || word[i + 1] == '_' || word[i + 1] == '?'))
		{
			i++;
			// 환경변수 이름 추출
			int var_start = i;
			while (word[i] && (ft_isalnum(word[i]) || word[i] == '_' || word[i] == '?'))
			{
				if (word[i] == '?')
				{
					i++;
					break;
				}
				i++;
			}
			
			// 환경변수 이름 복사
			char *var_name = shell_malloc(shell, i - var_start + 1);
			ft_strncpy(var_name, word + var_start, i - var_start);
			var_name[i - var_start] = '\0';
			
			// 값 가져오기
			char *var_value;
			if (ft_strcmp(var_name, "?") == 0)
			{
				char status[16];
				ft_itoa_n(status, sizeof(status), shell->status.exit_code);
				var_value = status;
			}
			else
			{
				var_value = get_env_value(shell, var_name);
			}
			
			// 값 복사
			if (var_value)
			{
				int var_len = ft_strlen(var_value);
				ft_strcpy(temp + j, var_value);
				j += var_len;
			}
			shell_free(shell, var_name);
		}
		else
		{
			temp[j++] = word[i++];
		}
	}
	
	temp[j] = '\0';
	result = shell_strdup(shell, temp);
	shell_free(shell, temp);
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
	process_word_content(input, word, *len);
	result = finalize_word(shell, word, quote_state);
	shell_free(shell, word);
	return (result);
}
