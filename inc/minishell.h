/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:21:40 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 02:19:19 by yutsong          ###   ########.fr       */
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
# include <sys/stat.h>

extern volatile sig_atomic_t	g_signal;

typedef struct s_status
{
	int	exit_code;
}	t_status;

// 추상 구문 트리(AST) 노드 타입
typedef enum e_ast_type {
    AST_COMMAND,     // 단일 명령어
    AST_PIPE,        // 파이프
} t_ast_type;

// 토큰 타입
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

// AST 노드 구조체
typedef struct s_ast_node
{
    t_ast_type          type;
    t_command           *cmd;          // 현재 명령어
    struct s_ast_node   *left;         // 파이프 왼쪽 명령어
    struct s_ast_node   *right;        // 파이프 오른쪽 명령어
} t_ast_node;

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
	t_ast_node	*ast_root;
	t_status	status;
	t_memory	*memory;
	char		*input_line;
	t_parser	parser;
	t_pipe		pipe;
	t_heredoc	heredoc;
	t_env		*env;
}	t_shell;

// memory.c
void	*shell_malloc(t_shell *shell, size_t size);
void	shell_free(t_shell *shell, void *ptr);
void	free_all_memory(t_shell *shell);

// env.c
t_env	*init_env(t_shell *shell, char **envp);
void	set_env_value(t_shell *shell, const char *key, const char *value);

// env_array.c
char **get_env_array(t_shell *shell);

// ft.c
char *ft_strchr(const char *str, int c);
int ft_strlen(const char *str);
char	*shell_strdup(t_shell *shell, const char *str);

// signal.c
void setup_signals(void);

// tokenizer.c
int tokenize_input(t_shell *shell);
t_token *create_token(t_shell *shell, char *value, t_token_type type);

// tokenizer_redir.c
t_token *handle_redirection(t_shell *shell, char **input);

// tokenizer_utils.c
char *handle_word(char *input, int *len);

// execute.c
int execute_commands(t_shell *shell);
int execute_ast(t_shell *shell, t_ast_node *node);

// execute_pipe.c
int execute_pipe(t_shell *shell, t_ast_node *node);
int execute_piped_command(t_shell *shell, t_command *cmd, int is_first, int is_last);
void setup_pipe(t_shell *shell, int pipefd[2], int is_first, int is_last);
void wait_all_children(t_shell *shell, int cmd_count);

// execute_utils.c
int is_builtin(char *cmd_name);
int execute_builtin(t_shell *shell, t_command *cmd);

// execute_built.c
int builtin_echo(char **args);
int builtin_cd(t_shell *shell, char **args);
int builtin_pwd(void);
int builtin_export(t_shell *shell, char **args);
int builtin_unset(t_shell *shell, char **args);
int builtin_env(t_shell *shell);
int builtin_exit(t_shell *shell, char **args);

// execute_extern.c
int execute_external(t_shell *shell, t_command *cmd);

// execute_path.c
char *find_command_path(t_shell *shell, const char *cmd);

// parser.c
int parse_input(t_shell *shell);
char **create_args_array(t_shell *shell, t_token *start, int arg_count);
t_command *create_command(t_shell *shell, t_token **tokens);

// parser_ast.c
t_ast_node *create_ast_node(t_shell *shell, t_ast_type type);

// parser_pipe.c
t_ast_node *parse_pipeline(t_shell *shell, t_token **tokens);

// parser_simple.c
t_ast_node *parse_simple_command(t_shell *shell, t_token **tokens);

// setup_redirections.c
int setup_redirections(t_shell *shell, t_redirection *redirs);

#endif