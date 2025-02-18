/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:21:40 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/18 07:59:16 by yutsong          ###   ########.fr       */
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
# include <errno.h>

extern volatile sig_atomic_t	g_signal;

typedef enum s_redirection_type
{
	REDIR_NONE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redirection_type;

typedef struct s_redirection
{
	t_redirection_type      type;       // REDIR_IN, REDIR_OUT 등
	char                    *filename;   // 파일명 또는 구분자
	struct s_redirection    *next;      // 다음 리다이렉션
} t_redirection;

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

// 파이프 실행 정보를 담는 구조체
typedef struct s_pipe_info
{
    int pipefd[2];     // 현재 파이프의 파일 디스크립터
    int prev_pipe;     // 이전 파이프의 읽기 fd
    int is_last;       // 마지막 명령어 여부
} t_pipe_info;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_command
{
	char            **args;      // 명령어와 인자들
	t_redirection   *redirs;     // 리다이렉션 링크드 리스트
} t_command;

// AST 노드 구조체
typedef struct s_ast_node
{
    t_ast_type          type;
    t_command           *cmd;          // 현재 명령어
    struct s_ast_node   *left;         // 파이프 왼쪽 명령어
    struct s_ast_node   *right;        // 파이프 오른쪽 명령어
} t_ast_node;


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

// typedef struct s_pipe
// {
// 	int	read_fd;
// 	int	write_fd;
// }	t_pipe;

// typedef struct s_heredoc
// {
// 	char	*delimiter;
// 	int		temp_fd;
// }	t_heredoc;

// 히어독 관리를 위한 구조체
typedef struct s_heredoc {
    int     original_stdin;  // 원본 표준 입력 저장
    int     fd;             // 현재 히어독의 파일 디스크립터
    char    *delimiter;      // 구분자
    char    *temp_file;     // 임시 파일 경로
} t_heredoc;

typedef struct s_parser
{
	char		*input;
	size_t		now_where;
	t_token		*curr_token;
}	t_parser;

typedef struct s_shell
{
	t_token		*tokens;
	// t_command	*commands;
	t_ast_node	*ast_root;
	t_status	status;
	t_memory	*memory;
	char		*input_line;
	t_parser	parser;
	t_pipe_info	pipe_info;
	t_heredoc	heredoc;
	t_env		*env;
}	t_shell;

// memory.c
void	*shell_malloc(t_shell *shell, size_t size);
void	shell_free(t_shell *shell, void *ptr);
void	free_all_memory(t_shell *shell);
void	free_command_memory(t_shell *shell);

// env.c
t_env	*init_env(t_shell *shell, char **envp);
void	set_env_value(t_shell *shell, const char *key, const char *value);
int	parse_env_arg(t_shell *shell, char *arg, char **key, char **value);
char *get_env_value(t_shell *shell, const char *key);
char *expand_env_var(t_shell *shell, const char *str);

// env_array.c
char **get_env_array(t_shell *shell);


// ft.c
char *ft_strchr(const char *str, int c);
int ft_strlen(const char *str);
char	*shell_strdup(t_shell *shell, const char *str);
char *ft_strdup(const char *str);
void	*ft_memset(void *dest, int c, size_t count);

// signal.c
void setup_signals(void);
void heredoc_signal_handler(int signo);

// tokenizer.c
int tokenize_input(t_shell *shell);
t_token *create_token(t_shell *shell, char *value, t_token_type type);

// tokenizer_redir.c
t_token *handle_redirection(t_shell *shell, char **input);

// tokenizer_utils.c
char *handle_word(t_shell *shell, char *input, int *len);
void add_token(t_shell *shell, t_token *token);

// execute.c
int execute_commands(t_shell *shell);
int execute_ast(t_shell *shell, t_ast_node *node);
int execute_simple_command(t_shell *shell, t_command *cmd);

// execute_pipe.c
int execute_pipe(t_shell *shell, t_ast_node *node);
int execute_piped_command(t_shell *shell, t_command *cmd);
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
char *find_executable(t_shell *shell, const char *cmd);

// execute_here.c
int handle_heredoc(t_shell *shell, char *delimiter);

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

// redir_utils.c
void add_redirection(t_redirection **redirs, t_redirection *new_redir);
t_redirection *create_redirection(t_shell *shell, t_token *token);

// debug_print.c
void debug_print(int action_combine_bit, int str_type, const char *str, ...);

// AST 실행 관련 함수
int execute_simple_command(t_shell *shell, t_command *cmd);

// 함수 선언 추가
char *create_temp_heredoc_file(t_shell *shell);
void cleanup_heredoc(t_shell *shell);

// AST 출력 관련 함수 선언 추가
void print_ast(t_ast_node *node, int depth);
void print_command(t_command *cmd);
void print_redirection(t_redirection *redir);

#endif
