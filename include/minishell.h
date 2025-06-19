/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:11:14 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/17 02:29:31 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// color macros
# include "colors.h"

// structs header
# include "structs.h"

// libft
# include "../libft/include/libft.h"

// to use printf
# include <stdio.h>

// to use malloc, free, exit
# include <stdlib.h>

// to use write, usleep, fork, getcwd, chdir, unlink, execve, dup, dup2, 
// pipe, access, read, close, isatty, ttyname, ttyslot, ioctl
# include <unistd.h>

// for O_* constants
# include <fcntl.h>

// to use pid_t type
# include <sys/types.h>

// to use waitpid, wait, wait3, wait4
# include <sys/wait.h>

// to use readline functions
# include <readline/readline.h>
# include <readline/history.h>

// to use errno and ENOENT
# include <errno.h>

// to use opendir, readdir and closedir
# include <dirent.h>

// to use signal functions
# include <signal.h>

// to use stat, lstat, fstat
# include <sys/stat.h>

// to use tcsetattr, tcgetattr
# include <termios.h>

// to use strerror
# include <string.h>

// to use perror (also in stdio.h)
// to use getenv
# include <stdlib.h>

// to use termcap functions
# include <term.h>

// to use ioctl
# include <sys/ioctl.h>

// =============================== main =======================================

// Main interactive prompt loop.
// 1. Generates and displays prompt.
// 2. Reads user input.
// 3. Handles signals and special commands.
// 4. Processes valid input.
void			prompt(t_shell *shell);

// Core minishell processing function.
// 1. Tokenizes input string.
// 2. Stores tokens in shell structure.
void			minishell(char *input, t_shell *shell);

// Parses tokens into executable structure.
// 1. Performs synxtax analysis.
// 2. Builds abstract syntax tree (AST).
// 3. Executes commands if no errors.
void			parser(t_token_list *tokens, t_shell *shell);

// Validates if input contains non-whitespace characters.
int				handle_all_white_spaces(char *input);

// =============================== tokenizer ==================================

// Converts input string into a list of tokens.
// 1. Initializes token list structure.
// 2. Processes each character sequentially.
// 3. Handles words, operators and whitespaces.
// 4. Returns NULL on failure.
t_token_list	*tokenizer(char *input);

// Processes single input character and delegates handling.
// 1. Skips whitespace characters.
// 2. Handles operators via process_operator.
// 3. Processes words via handle_word.
// Returns 0 on error, 1 on success, 2 when operator handled.
int				process_input_char(char *input, int *i, t_token_list *tokens);

// Handles operator characters in input.
// 1. Delegates to handle_operators.
// 2. Returns appropriate status code.
int				process_operator(char *input, int *i, t_token_list *tokens);

// Processes and creates word tokens.
// 1. Finds word boundaries.
// 2. Handles quoted sections.
// 3. Creates and appends new word token.
int				handle_word(char *input, int *i, t_token_list *tokens);

// Handles quoted sections within words.
// 1. Detects quote start.
// 2. Skips until matching quote.
// 3. Validates proper quote termination.
int				handle_quotes(char *input, int *j);

// Handles operator characters in input.
// 1. Checks if current character is an operator.
// 2. Creates operator token if valid.
// 3. Reports errors for invalid operators.
// Returns 1 if operator handled, 0 on error, -1 if not operator.
int				handle_operators(char *input, int *i, t_token_list *tokens);

// Creates and appends operator token to list.
// 1. Determines operator type and length.
// 2. Extracts operator substring.
// 3. Creates and appends new token.
// Returns 1 on success, 0 if no operator found.
int				create_op_token(char *input, int *i, t_token_list *tokens);

// Checks for two-character operators.
// 1. Tests for >>, <<, && and ||.
// 2. Sets type and length if found.
// Returns 1 if match found, 0 otherwise.
int				is_two_char_op(char *input, t_token_type *type, int *len);

// Checks for single-character operators (part 1).
// 1. Tests for >, <, |.
// 2. Sets type and length if found.
// 3. Delegates remaining checks to is_one_char_op_2
// Returns 1 if match found, 0 otherwise.
int				is_one_char_op_1(char input, t_token_type *type, int *len);

// Checks for single-character operators (part 1).
// 1. Tests for (, ).
// 2. Sets type if found.
// Returns 1 if match found, 0 otherwise.
int				is_one_char_op_2(char input, t_token_type *type);

// Checks if a character exists in a string.
// 1. Iterates through string s to find character c.
int				in(const char *s, char c);

// Creates a new token.
// 1. Allocates a t_token.
// 2. Sets type and value.
// 3. Initializes next to NULL.
t_token			*new_token(t_token_type type, char *value);

// Initializes t_token list struct.
void			token_list_init(t_token_list *list);

// Appends token to the end of the list.
// 1. Links new token to tail if list is not empty.
// 2. Updates head if list was empty.
// 3. Updates tail and increments size.
void			token_list_append(t_token_list *list, t_token *token);

// Frees all tokens in the list and the list itself.
// 1. Iterates through each token.
// 2. Frees token value and token struct.
// 3. Frees the list structure.
void			token_list_free(t_token_list *list);

// =============================== env ========================================

// Initializes a linked list of environment variables from envp.
// 1. Iterates over each entry in envp.
// 2. Process each entry with env_process() to create a node.
// 3. Returns the fully constructed list.
t_env			*init_env(char **envp);

// Process a single environment variable string.
// 1. Splits the string into key and value at '=" character.
// 2. Creates and appends a new node to the list.
int				env_process(char *entry, t_env **head, t_env **tail);

// Creates new environment variable node.
// 1. Allocates and initializes a new t_env node.
// 2. Duplicates key and value strings.
t_env			*env_new(const char *key, const char *value);

// Appends a node to the end of the t_env list.
// 1. Adds to head if list is empty.
// 2. Otherwise appends to tail.
int				env_append(t_env **head, t_env **tail, t_env *node);

// Creates empty environment variable if nonexistent.
// 1. Checks if variable already exists.
// 2. Appends new node with NULL value if not found.
void			env_export_only(t_env **env_head, char *name);

// Retrieves value for environment variable key.
char			*get_env_value(t_env *env, const char *key);

// Gets PATH environment variable value.
char			*get_env_path(t_env *env);

// Updates existing environment variable or creates new.
// 1. Finds and updates matching key.
// 2. Creates new entry if key was not found.
void			env_update(t_env **env, char *key, char *value);

// Creates and appends new environment variable.
void			env_createapnd(t_env **head, char *key, char *val, t_env *curr);

// Converts environment linked list to a NULL-terminated array of strings.
// 1. Counts exported variables (those with values).
// 2. Allocates and fills array with "KEY=VALUE" strings.
// 3. Returns the array for use with execve.
// Note: Exits on malloc or ft_strjoin3 failure.
char			**env_list_to_array(t_env *env);

// Frees a NULL-terminated array of strings.
// 1. Frees each string in the array.
// 2. Frees the array pointer itself.
void			free_string_array(char **arr);

// =============================== clean ======================================

void			clean_all(t_token_list *tokens, t_ast *node, t_env **env);
void			report_unexpected_quotes(const char token_value);
int				report_unexpected(const char *token_value);

// clean env
void			env_free_all(t_env **head);

// clean ast
void			free_args(t_command *command);
void			command_free(t_command *command);
void			ast_free(t_ast *root);

// clean expansion
void			free_args_temp(char **args);
void			free_args_list(t_args *args);
void			free_array(void **ptr);

// =============================== expansion ==================================

void			expander(char ***args, t_env *env, int *size_args, t_shell *sh);
char			**list_to_args(t_args *args, int *size_args);

// expand env
t_args			*expand_env(char **args, t_env *env, t_shell *sh);
char			*start_prefix(char *input, int *i, t_quote *state);
void			expand_variable(t_exp *exp, char *key);
void			expand_status(t_exp *exp, int status);
void			handle_no_quotes(t_exp *exp);
char			*extract_key(char *input, int *i);
void			update_state_quote(t_quote *state, char c);
char			*find_env_value(char *key, t_env *env);
char			*ft_strjoin_free(char *s1, char *s2);

// expand token
t_args			*expand_token(char *input, t_env *env, t_shell *sh);
void			init_expander(t_exp *exp, char *input, t_env *env);
void			handle_next_token(t_exp *exp, char *input, t_shell *sh);

// expansion utils
void			add_token(t_args **head, char *value);
void			append_list(t_args **head, t_args *list);
int				count_array(void **arr);
void			add_token_free(t_args **head, char *value);

// expand wild
t_args			*expand_wild(t_args *envs);
t_wild			parse_pattern(char *arg);
t_args			*wild_matches(t_wild *wild);
int				is_wildcard(char *arg);
int				only_asterisk(char *arg);
int				match_middle(char *pos, t_wild *wild, int i, int limit);
int				match_end(const char *filename, const char *end);
int				match_pattern(const char *filename, t_wild *wild);
int				handle_opening_quote(char *input, int *i, t_quote *state);
int				update_state_quote_i(char *input, int *i, t_quote *state);
char			**split_wildcard(char *input);
void			add_token_to_array(char ***array, char *token);

// sort wildcard
void			swap_args(t_args *a, t_args *b);
int				ft_tolower(int c);
int				ft_strcasecmp(const char *s1, const char *s2);
void			sort_args_list(t_args *list);
void			sort_append(t_args **head, t_args *list);

// expand redirs
void			expand_redirs(t_redir *redirs, t_shell *sh);
void			expand_redir(t_redir *redir, t_shell *sh);
t_args			*expand_redir_wild(t_args *envs);
int				count_args(t_args *list);
void			handle_no_match(t_shell *sh, t_redir *redir);
void			handle_ambiguous(t_shell *sh, t_redir *redir);
void			handle_single_match(t_redir *redir, t_args *expanded_list);

// =============================== utils ======================================

void			exit_perror(const char *msg);
char			*rem_quotes(char *str, int free_str);

// =============================== ast ========================================

t_ast			*parse_command(t_token **token, t_shell *shell);
t_ast			*parse_logical(t_token **token, t_shell *shell);
t_ast			*parse_pipe(t_token **token, t_shell *shell);
t_ast			*parse_simple_command(t_token **token, t_shell *shell);
t_ast			*parse_subshell(t_token **token, t_shell *shell);
int				is_pipe_or_logical(t_token *token);
int				is_redirect(t_token *token);
int				get_size_args(t_token **token);
int				have_quotes(char *raw);

// commands
t_command		*make_command(t_token **token, t_shell *shell);
t_command		*init_command(void);
void			parse_redirect(t_token **token, t_command **cmd, t_shell *sh);
void			parse_heredoc(t_token **token, t_command **cmd, t_shell *shell);
char			**get_args(t_token **token, int size_args);
void			append_redir(t_redir **head, t_redir_type type, char *filename);

// ======================== syntax analysis ===================================

int				syntax_analysis(t_token_list *tokens);
int				check_pipe(t_token *prev, t_token *next);
int				check_redir(t_token *next);
int				check_logical(t_token *prev, t_token *token, t_token *next);
int				is_twochar(t_token *token);
int				check_paren(t_token *p, t_token *t, t_token *n, int *depth);
int				is_all_whitespace(const char *s);

// =============================== executor ===================================

void			executor(t_token_list *tokens, t_shell *sh, t_ast *node);
int				exec_dispatch(char **args, t_shell *shell, char **envp);
void			execve_with_path(char **args, t_env *env, char **envp);
void			try_exec_explicit(char *cmd, char **args, char **envp);
int				is_builtin(char *cmd);
int				run_builtin(char **args, t_shell *shell);
int				is_executable_command(char *cmd, t_env *env);
int				is_explicit_executable(char *cmd);
int				search_in_path(char *cmd, t_env *env);
int				try_exit(t_token_list *toks, char **args, t_shell *shell);
int				try_other_builtin(char **args, t_command *cmd, t_shell *shell);
void			run_external_cmd(char **args, t_command *cmd, t_shell *shell);
int				is_explicit_dir(char *cmd);
void			handle_dir_case(char *cmd, t_shell *shell);
void			handle_not_found(char *cmd, t_shell *shell);
void			handle_permission_denied(char *cmd, t_shell *shell);
int				exec_with_redirs(char **args, t_command *cmd, t_shell *shell);
void			handle_left_child(int *fd, t_shell *shell, t_ast *node);
void			handle_right_child(int *fd, t_shell *shell, t_ast *node);

// redir
int				apply_redirections(t_command *cmd, t_shell *sh);
int				handle_in_redir(t_redir *redir, int *in_fd, t_shell *sh);
int				handle_heredoc(t_redir *redir, int *in_fd);
int				handle_out_redir(t_redir *redir, int *out_fd, t_shell *sh);
void			dup2_close(int fd, int std_fd);

// heredoc
void			prepare_heredocs(t_ast *node, t_shell *sh);
int				process_hd(char *delim, int no_expand, t_env *env, t_shell *sh);
char			*get_var_value(char *line, int *ip, t_env *env, int last_sts);
char			*ft_strjoin_char_free(char *str, char c);

// stdin stdout
int				save_fds(int *save_stdin, int *save_stdout);
int				restore_fds(int save_stdin, int save_stdout);

// last status
void			set_last_status(t_shell *shell, int status);
int				get_last_status(t_shell *shell);

// =============================== builtin ====================================

void			builtin_exit(t_token_list *tokens, t_shell *shell);
int				parse_exit_code(char *arg_str, t_shell *shell);
int				builtin_pwd(t_env *env);
int				builtin_env(t_env *env);
int				builtin_echo(char **args);
int				builtin_cd(char **args, t_env *env);
int				builtin_export(char **args, t_env **env);
void			print_exported(t_env *env);
int				builtin_unset(char **args, t_env **env);

// =============================== signals ====================================

// signals prompt
void			setup_signals_prompt(void);
t_sig			setup_sigint_prompt(void);
void			handle_sigint_prompt(int sig);
void			update_sh_last_status(t_shell *sh, int new_value);

// signals heredoc
void			handle_sigeof_heredoc(char *delim);
void			handle_sigint_heredoc(int sig);
void			setup_signals_heredoc(void);
void			disable_echoctl(void);
void			enable_echoctl(void);

// signals exec
void			setup_signals_exec(void);
t_sig			setup_sigint_exec(void);
t_sig			setup_sigquit_exec(void);
void			handle_sigint_exec(int sig);
void			handle_sigquit_exec(int sig);

// =============================== prompt =====================================
char			*make_prompt(t_shell *shell);
char			*get_user(void);
char			*read_hostname_file(void);
char			*get_cwd_display(void);
void			build_user_host(char *dst, size_t *i, char *user, char *host);
void			build_cwd(char *dst, size_t *i, char *cwd);
void			append_str(char *dst, size_t *i, const char *src);
void			append_char(char *dst, size_t *i, char c);
char			*replace_home(char *cwd, char *home);

#endif
