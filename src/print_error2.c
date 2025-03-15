/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 05:54:11 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/15 08:25:26 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_file_not_found(va_list args)
{
	char	*cmd;

	cmd = va_arg(args, char *);
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": No such file or directory\n", 28);
}

void	handle_permission_denied(va_list args)
{
	char	*cmd;

	cmd = va_arg(args, char *);
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": Permission denied\n", 20);
}

void	handle_command_not_found(va_list args)
{
	char	*cmd;

	cmd = va_arg(args, char *);
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": command not found\n", 20);
}

void	handle_export_error(va_list args)
{
	char	*identifier;

	identifier = va_arg(args, char *);
	write(STDERR_FILENO, "minishell: export: `", 20);
	write(STDERR_FILENO, identifier, ft_strlen(identifier));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
}

void	handle_custom_error(va_list args)
{
	char	*cmd;
	char	*error_msg;

	cmd = va_arg(args, char *);
	error_msg = va_arg(args, char *);
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	write(STDERR_FILENO, "\n", 1);
}
