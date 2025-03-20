/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:21:40 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 06:44:41 by yutsong          ###   ########.fr       */
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
# include <termios.h>
# include <sys/ioctl.h>
# include <stdarg.h>

extern volatile sig_atomic_t	g_signal;

typedef enum e_char_type
{
	QUOTE_CHAR,
	ENV_VAR,
	REGULAR_CHAR
}	t_char_type;

typedef struct s_token_state
{
	int	in_single_quote;
	int	in_double_quote;
}	t_token_state;

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
	t_redirection_type		type;
	char					*filename;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_status
{
	int	exit_code;
}	t_status;

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
}	t_ast_type;

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR,
	TOKEN_ENV,
	TOKEN_EOF
}	t_token_type;

typedef struct s_pipe_info
{
	int	pipefd[2];
	int	prev_pipe;
	int	is_last;
}	t_pipe_info;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_command
{
	char			**args;
	t_redirection	*redirs;
}	t_command;

typedef struct s_ast_node
{
	t_ast_type			type;
	t_command			*cmd;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

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

typedef struct s_heredoc_entry
{
	char					*delimiter;
	char					*temp_file;
	int						fd;
	struct s_heredoc_entry	*next;
}	t_heredoc_entry;

typedef struct s_heredoc
{
	t_heredoc_entry	*entries;
	int				count;
	int				original_stdin;
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
	t_ast_node	*ast_root;
	t_status	status;
	t_memory	*memory;
	char		*input_line;
	t_parser	parser;
	t_pipe_info	pipe_info;
	t_heredoc	heredoc;
	t_env		*env;
	int			original_stdout;
	int			original_stderr;
}	t_shell;

typedef struct s_tree
{
	struct s_tree	*left_child;
	struct s_tree	*right_sibling;
	char			*value;
}	t_tree;
char			**get_env_array(t_shell *shell);

void			expand_variable(t_shell *shell,
					char **write_pos, const char **str);
int				should_expand_var(const char *str, int in_single_quote);
char			*expand_env_var(t_shell *shell, const char *str);
void			expand_exit_code(t_shell *shell,
					char **write_pos, const char **str);

int				parse_env_arg(t_shell *shell,
					char *arg, char **key, char **value);
void			add_env_value(t_shell *shell,
					const char *key, const char *value);
void			set_env_value(t_shell *shell,
					const char *key, const char *value);
t_env			*create_env_node(t_shell *shell, char *key, char *value);
void			add_env_node(t_env **env_list, t_env *new_node);

char			*extract_var_name(t_shell *shell, const char **str);
char			*handle_special_var(t_shell *shell, const char *var_name);

t_env			*init_env(t_shell *shell, char **envp);
char			*get_env_value(t_shell *shell, const char *key);
void			handle_single_quote(const char **str, int *in_single_quote);
void			copy_char(char **write_pos, const char **str);

void			remove_env_var(t_shell *shell, const char *key);
int				builtin_unset(t_shell *shell, char **args);
int				builtin_env(t_shell *shell);
int				builtin_exit(t_shell *shell, char **args);

int				builtin_echo(char **args);
int				builtin_cd(t_shell *shell, char **args);
int				builtin_pwd(void);
int				builtin_export(t_shell *shell, char **args);

int				execute_external(t_shell *shell, t_command *cmd);
int				handle_signal_termination(t_shell *shell, int status);

void			execute_command_in_child(t_shell *shell, t_command *cmd);

char			*create_temp_heredoc_file(t_shell *shell);
t_heredoc_entry	*create_heredoc_entry(t_shell *shell, char *delimiter);
int				setup_heredoc_file(t_shell *shell, char **temp_file, int *fd);
t_heredoc_entry	*setup_heredoc_entry(t_shell *shell, char *delimiter,
					char *temp_file, int *fd);
void			check_heredoc_signal(int fd);

void			handle_null_line(t_shell *shell, int fd);
void			check_delimiter_match(char *line, char *delimiter,
					int fd, t_shell *shell);
void			process_heredoc_lines(t_shell *shell, char *delimiter, int fd);

int				read_heredoc_content(t_shell *shell, char *delimiter, int fd);
int				setup_heredoc_read(t_shell *shell,
					t_heredoc_entry *entry, char *temp_file);
int				handle_heredoc(t_shell *shell, char *delimiter);
int				get_heredoc_fd(t_shell *shell, char *delimiter);

void			free_heredoc_entries(t_shell *shell);
void			cleanup_heredoc(t_shell *shell);
int				find_command_heredoc_fd(t_shell *shell, t_command *cmd);
void			setup_command_heredoc(t_shell *shell, t_command *cmd);

char			*get_path_value(t_shell *shell);
int				count_path_segments(char *path_str);
int				extract_path_seg(t_shell *shell, char **paths,
					char *path_str, int count);
char			**split_path(t_shell *shell, char *path_str);

int				is_executable(const char *path);
char			*search_in_path(t_shell *shell,
					const char *cmd, char *path_copy);
char			*find_executable(t_shell *shell, const char *cmd);
char			*search_command_in_paths(t_shell *shell,
					const char *cmd, char **paths);
char			*find_command_path(t_shell *shell, const char *cmd);
char			*handle_direct_path(t_shell *shell, char *cmd_path);
char			*handle_path_search(t_shell *shell, char *cmd_name);

int				handle_heredocs_recursive(t_shell *shell, t_ast_node *node);
void			setup_pipe(t_shell *shell, int pipefd[2],
					int is_first, int is_last);
void			execute_in_pipe(t_shell *shell, t_command *cmd);
int				execute_piped_command(t_shell *shell, t_command *cmd);
int				handle_all_heredocs(t_shell *shell, t_ast_node *node);

void			execute_left_command(t_shell *shell,
					t_ast_node *node, int pipefd[2]);
void			execute_external_command(t_shell *shell, t_command *cmd);
void			execute_right_command(t_shell *shell,
					t_ast_node *node, int pipefd[2]);
void			setup_right_command_io(t_shell *shell, t_command *cmd);
int				execute_pipe(t_shell *shell, t_ast_node *node);

void			setup_left_command_io(t_shell *shell, t_command *cmd);

int				is_builtin(char *cmd_name);
int				execute_builtin(t_shell *shell, t_command *cmd);
void			wait_all_children(t_shell *shell, int cmd_count);

int				execute_simple_command(t_shell *shell, t_command *cmd);
int				process_heredocs(t_shell *shell, t_redirection *redirs);
int				process_redirection(t_shell *shell, t_redirection *redirs);
int				execute_ast(t_shell *shell, t_ast_node *node);
int				execute_commands(t_shell *shell);

char			*ft_strchr(const char *str, int c);
int				ft_strlen(const char *str);
char			*shell_strdup(t_shell *shell, const char *str);
char			*ft_strdup(const char *str);
void			*ft_memset(void *dest, int c, size_t count);

int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strncpy(char *dest, const char *src, size_t n);
int				ft_isalnum(int c);
int				ft_isdigit(int c);
int				ft_isspace(int c);

int				ft_atoi(const char *str);
char			*ft_strcat(char *dest, const char *src);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				handle_special_cases(char *str, size_t size, int n);

int				ft_itoa_n(char *str, size_t size, int n);

void			ft_itoa_simple(char *str, int n);

char			*ft_strtok(char *str, const char *delim);

int				ft_isalpha(int c);
int				is_valid_identifier(char *str);
void			update_target_env_with_concat(t_env *target_env,
					char *new_part, t_shell *shell);
void			process_export_value(t_env *input_env, t_env *env_head,
					char *str, t_shell *shell);
void			handle_export_values(t_tree *keyvalue_node, t_env *env_head,
					t_shell *shell);

t_env			*set_input_env(char *str, t_shell *shell);
void			stretch_value(t_env *input_env, t_env *env_head,
					t_shell *shell);
char			*dup_val(char *value, t_shell *shell);

void			*ft_memcpy(void *dest, const void *src, size_t count);
char			*ft_strndup(const char *s, size_t n, t_shell *shell);
char			*ft_strnstr(const char *big, const char *little, size_t len);
char			*ft_strjoin(char const *s1, char const *s2, t_shell *shell);

void			*shell_malloc(t_shell *shell, size_t size);
void			shell_free(t_shell *shell, void *ptr);
void			free_command_memory(t_shell *shell);

int				is_env_memory(t_shell *shell, void *addr);
void			free_all_memory(t_shell *shell);
void			free_shell_malloc(t_shell *shell);
void			free_env(t_shell *shell);
void			free_exit(t_shell *shell, int status);

t_ast_node		*create_ast_node(t_shell *shell, t_ast_type type);

t_ast_node		*parse_pipeline(t_shell *shell, t_token **tokens);

t_ast_node		*parse_simple_command(t_shell *shell, t_token **tokens);

int				parse_input(t_shell *shell);

int				count_args(t_token *curr);
char			**create_args_array(t_shell *shell,
					t_token *start, int arg_count);
void			add_redirection(t_redirection **redirs,
					t_redirection *new_redir);
t_redirection	*process_redirections(t_shell *shell, t_token **tokens);
t_command		*create_command(t_shell *shell, t_token **tokens);

void			setup_signals_interactive(void);
void			setup_signals_executing(void);
void			setup_signals_heredoc(void);
void			setup_signals(void);

void			interactive_signal_handler(int signo);
void			executing_signal_handler(int signo);
void			heredoc_signal_handler(int signo);

t_token			*create_red_token(t_shell *shell,
					const char *value, t_token_type type);
t_token			*create_word_token(t_shell *shell,
					char **input, t_token *token);
t_token			*handle_heredoc_redirection(t_shell *shell, char **input);
t_token			*handle_regular_redirection(t_shell *shell, char **input);
t_token			*handle_redirection(t_shell *shell, char **input);

int				handle_quote_character(char c,
					t_token_state *state, int *dollar_sign);
int				is_word_delimiter(char c, t_token_state state);
int				get_word_length(char *input);
int				determine_quote_state(char *input, int *start_idx);
void			process_word_content(char *input, char *word, int len);

char			*finalize_word(t_shell *shell, char *word, int quote_state);
char			*handle_word(t_shell *shell, char *input, int *len);

t_token			*create_token(t_shell *shell, char *value, t_token_type type);
void			add_token(t_shell *shell, t_token *token);
int				tokenize_input(t_shell *shell);

t_token			*create_token(t_shell *shell, char *value, t_token_type type);
void			add_token(t_shell *shell, t_token *token);
int				handle_pipe_token(t_shell *shell, char **input);
int				handle_word_token(t_shell *shell, char **input);
char			*skip_spaces(char *input);

int				process_next_token(t_shell *shell, char **input);
int				tokenize_input(t_shell *shell);

int				execute_commands(t_shell *shell);
int				execute_ast(t_shell *shell, t_ast_node *node);
int				execute_simple_command(t_shell *shell, t_command *cmd);

int				is_builtin(char *cmd_name);
int				execute_builtin(t_shell *shell, t_command *cmd);

char			*find_command_path(t_shell *shell, const char *cmd);
char			*find_executable(t_shell *shell, const char *cmd);

t_redirection	*create_redirection(t_shell *shell, t_token *token);

int				execute_simple_command(t_shell *shell, t_command *cmd);

void			setup_signals_interactive(void);
void			setup_signals_executing(void);
void			setup_signals_heredoc(void);
void			heredoc_signal_handler(int signo);
void			setup_signals(void);

void			restore_io(t_shell *shell);
void			free_non_heredoc_memory(t_shell *shell);

t_tree			*convert_args_to_node(char **args, t_tree *node);
t_env			*find_already(char *key, t_env *env_head);

void			add_keyvalue(t_env *input_env, t_env *env_head);
char			*get_env(t_env *head, char *key);
t_env			*find_already(char *key, t_env *env_head);
void			export_for_cd(char *key, char *value, t_env **env_head,
					t_shell *shell);

void			update_quote_state(char *word, int *i, int *in_single_quote,
					int *in_double_quote);
void			expand_env_var_to_res(t_shell *shell,
					char *word, char *temp, int indices[2]);
void			process_word(t_shell *shell, char *word, char *temp, int *j);

int				backup_original_fds(t_shell *shell);
void			cleanup_backup_fds(t_shell *shell);
int				open_output_file(t_redirection *redir);
int				setup_redirections(t_shell *shell, t_redirection *redirs);

int				handle_input_redirections(t_shell *shell,
					t_redirection *redirs);

int				handle_output_redirections(t_shell *shell,
					t_redirection *redirs);

void			exit_setup(t_shell *shell);
void			exit_loop(t_shell *shell);

int				process_single_input(t_redirection *current,
					int *last_fd, t_shell *shell);
#endif