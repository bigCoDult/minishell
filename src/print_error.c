/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 05:43:41 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 05:54:38 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_file_not_found(va_list args)
{
	char	*cmd;

	cmd = va_arg(args, char *);
	write_str(STDERR_FILENO, "minishell: ");
	write_str(STDERR_FILENO, cmd);
	write_str(STDERR_FILENO, ": No such file or directory\n");
}

static void	handle_permission_denied(va_list args)
{
	char	*cmd;

	cmd = va_arg(args, char *);
	write_str(STDERR_FILENO, "minishell: ");
	write_str(STDERR_FILENO, cmd);
	write_str(STDERR_FILENO, ": Permission denied\n");
}

static void	handle_command_not_found(va_list args)
{
	char	*cmd;

	cmd = va_arg(args, char *);
	write_str(STDERR_FILENO, "minishell: ");
	write_str(STDERR_FILENO, cmd);
	write_str(STDERR_FILENO, ": command not found\n");
}

static void	handle_custom_error(va_list args)
{
	char	*cmd;
	char	*error_msg;

	cmd = va_arg(args, char *);
	error_msg = va_arg(args, char *);
	write_str(STDERR_FILENO, "minishell: ");
	write_str(STDERR_FILENO, cmd);
	write_str(STDERR_FILENO, ": ");
	write_str(STDERR_FILENO, error_msg);
	write_str(STDERR_FILENO, "\n");
}

void	print_error(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	if (ft_strcmp(format, "minishell: %s: No such file or directory\n") == 0)
		handle_file_not_found(args);
	else if (ft_strcmp(format, "minishell: %s: Permission denied\n") == 0)
		handle_permission_denied(args);
	else if (ft_strcmp(format, "minishell: %s: command not found\n") == 0)
		handle_command_not_found(args);
	else if (ft_strcmp(format, "minishell: %s: %s\n") == 0)
		handle_custom_error(args);
	else
		write_str(STDERR_FILENO, format);
	va_end(args);
}
