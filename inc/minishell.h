/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:21:40 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 00:23:48 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

extern volatile sig_atomic_t	g_signal;

typedef struct s_status
{
	int	exit_code;
}	t_status;

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR,
	TOKEN_ENV,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_command
{
	char					*name;
	char					**args;
	struct s_redirection	*redirs;
	struct s_command		*pipe_next;
}	t_command;

typedef enum s_redirection_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redirection_type;

typedef struct s_redirection
{
	t_redirection_type		type;
	char					*filename;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_memory
{
	void			*addr;
	struct s_memory	*next;
}	t_memory;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_pipe
{
	int	read_fd;
	int	write_fd;
}	t_pipe;

typedef struct s_heredoc
{
	char	*delimiter;
	int		temp_fd;
}	t_heredoc;

typedef struct s_parser
{
	char		*input;
	size_t		now_where;
	t_token		*curr_token;
}	t_parser;

typedef struct s_shell
{
	t_token		*tokens;
	t_command	*commands;
	t_status	status;
	t_memory	*memory;
	char		*input_line;
	t_parser	parser;
	t_pipe		pipe;
	t_heredoc	heredoc;
	t_env		*env;
}	t_shell;

#endif