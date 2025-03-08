/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:22:42 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/08 16:28:02 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	n_option;

	n_option = 0;
	i = 1;
	// 여러 개의 -n 옵션(-n, -nn, -nnn 등) 처리
	while (args[i] && args[i][0] == '-')
	{
		int j = 1;
		// 첫 문자가 '-'이고 나머지가 모두 'n'인지 확인
		while (args[i][j] == 'n')
			j++;
		
		// 문자열이 "-n..." 형태이고 모든 문자가 'n'인 경우에만 옵션으로 인식
		if (j > 1 && args[i][j] == '\0')
		{
			n_option = 1;
			i++;
		}
		else
			break; // 'n'이 아닌 다른 문자가 있으면 옵션이 아님
	}
	
	// 인자 출력
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	
	// -n 옵션이 없으면 개행 출력
	if (!n_option)
		printf("\n");
	return (0);
}

int	builtin_cd(t_shell *shell, char **args)
{
	char	*path;
	char	current_path[1024];

	if (!args[1])
		path = getenv("HOME");
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		printf("cd: %s: No such file or directory\n", path);
		return (1);
	}
	if (getcwd(current_path, sizeof(current_path)))
		set_env_value(shell, "PWD", current_path);
	return (0);
}

int	builtin_pwd(void)
{
	char	current_path[1024];

	if (getcwd(current_path, sizeof(current_path)))
	{
		printf("%s\n", current_path);
		return (0);
	}
	return (1);
}

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
