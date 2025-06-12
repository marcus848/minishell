/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:11:14 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/10 00:51:07 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// color macros
# include "colors.h"

// libft
# include "../libft/include/libft.h"

// to use printf
# include <stdio.h>

// to use malloc, free, exit
# include <stdlib.h>

// to use write, usleep, fork
# include <unistd.h>

// for O_* constants
# include <fcntl.h>

// to use pid_t type
# include <sys/types.h>

// to use waitpid
# include <sys/wait.h>

// to use readline
# include <readline/readline.h>

// to use add_history
# include <readline/history.h>

// to use errno and ENOENT
# include <errno.h>

// to use opendir, readdir and closedir
# include <dirent.h>

// to use signal functions
# include <signal.h>

typedef struct sigaction	t_sig;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	LOGICAL_AND,
	LOGICAL_OR,
	PAREN_OPEN,
	PAREN_CLOSE,
}	t_token_type;

typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote;

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef enum s_redir_type
{
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	int				heredoc_fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char			**args;
	int				arg_count;
	t_redir			*redirs;
	int				heredoc_fd;
	int				is_builtin;
}	t_command;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_token_list
{
	t_token	*head;
	t_token	*tail;
	int		size;
}	t_token_list;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	t_command		*cmd;
}	t_ast;

typedef struct s_args
{
	char			*arg;
	struct s_args	*next;
}	t_args;

typedef struct s_expand
{
	t_args	**head;
	t_quote	state;
	t_env	*env;
	char	*prefix;
	char	*cur;
	int		i;
}	t_exp;

typedef struct s_wild
{
	char	**parts;
	int		have_start;
	int		have_end;
	int		full;
}	t_wild;

typedef struct s_shell
{
	t_env			*env;
	t_token_list	*tokens;
	t_ast			*ast;
	int				last_status;
}	t_shell;

typedef struct s_quote_ctx
{
	char	*str;
	char	*result;
	int		i;
	int		j;
	t_quote	state;
}	t_quote_ctx;

extern volatile int			g_signal_status;

// tokenizer
t_token_list	*tokenizer(char *input);
int				handle_operators(char *input, int *i, t_token_list *tokens);

// token
t_token			*new_token(t_token_type type, char *value);

// token list
void			token_list_init(t_token_list *list);
void			token_list_append(t_token_list *list, t_token *token);
void			token_list_free(t_token_list *list);

// env
t_env			*init_env(char **envp);
char			*get_env_value(t_env *env, const char *key);
char			*get_env_path(t_env *env);
t_env			*env_new(const char *key, const char *value);
int				env_append(t_env **head, t_env **tail, t_env *node);
void			env_update(t_env **env, char *key, char *value);
void			env_export_only(t_env **env_head, char *name);

// clean
void			clean_all(t_token_list *tokens, t_ast *node, t_env **env);
void			env_free_all(t_env **head);
void			exit_perror(const char *msg);
void			report_unexpected_quotes(const char token_value);
int				report_unexpected(const char *token_value);

// clean_ast
void			free_args(t_command *command);
void			command_free(t_command *command);
void			ast_free(t_ast *root);

// clean_expansion
void			free_args_temp(char **args);
void			free_args_list(t_args *args);
void			free_array(void **ptr);

// expansion
void			expander(char ***args, t_env *env, int *size_args, t_shell *sh);
char			**list_to_args(t_args *args, int *size_args);

// expand_env
t_args			*expand_env(char **args, t_env *env, t_shell *sh);
char			*start_prefix(char *input, int *i, t_quote *state);
void			expand_variable(t_exp *exp, char *key);
void			expand_status(t_exp *exp, int status);
void			handle_no_quotes(t_exp *exp);

//expand_token
t_args			*expand_token(char *input, t_env *env, t_shell *sh);
void			init_expander(t_exp *exp, char *input, t_env *env);
void			handle_next_token(t_exp *exp, char *input, t_shell *sh);

// expand_env_utils
char			*extract_key(char *input, int *i);
void			update_state_quote(t_quote *state, char c);
char			*find_env_value(char *key, t_env *env);
char			*ft_strjoin_free(char *s1, char *s2);

// expansion_utils
void			add_token(t_args **head, char *value);
void			append_list(t_args **head, t_args *list);
int				count_array(void **arr);
void			add_token_free(t_args **head, char *value);

// expand_wild
t_args			*expand_wild(t_args *envs);
t_wild			parse_pattern(char *arg);
t_args			*wild_matches(t_wild *wild);
int				is_wildcard(char *arg);
int				only_asterisk(char *arg);

// expand_wild_utils
int				match_middle(char *pos, t_wild *wild, int i, int limit);
int				match_end(const char *filename, const char *end);
int				match_pattern(const char *filename, t_wild *wild);
int				handle_opening_quote(char *input, int *i, t_quote *state);
int				update_state_quote_i(char *input, int *i, t_quote *state);

// expand_wild_split
char			**split_wildcard(char *input);
void			add_token_to_array(char ***array, char *token);

// handle_quotes
char			*rem_quotes(char *str, int free_str);

// sort_wildcard
void			swap_args(t_args *a, t_args *b);
int				ft_tolower(int c);
int				ft_strcasecmp(const char *s1, const char *s2);
void			sort_args_list(t_args *list);
void			sort_append(t_args **head, t_args *list);

// ast
t_ast			*parse_command(t_token **token, t_shell *shell);
t_ast			*parse_logical(t_token **token, t_shell *shell);
t_ast			*parse_pipe(t_token **token, t_shell *shell);
t_ast			*parse_simple_command(t_token **token, t_shell *shell);
t_ast			*parse_subshell(t_token **token, t_shell *shell);

// commands
t_command		*make_command(t_token **token, t_shell *shell);
t_command		*init_command(void);
void			parse_redirect(t_token **token, t_command **cmd, t_shell *sh);
void			parse_heredoc(t_token **token, t_command **cmd, t_shell *shell);
char			**get_args(t_token **token, int size_args);
void			append_redir(t_redir **head, t_redir_type type, char *filename);

// ast_utils
int				is_pipe_or_logical(t_token *token);
int				is_redirect(t_token *token);
int				get_size_args(t_token **token);

// debug functions
void			print_env(t_env *env);
void			print_envp(char **envp);
void			print_tokens(t_token_list *tokens);
void			print_token(char *str_type, t_token *token);
void			test_expander(t_env *env);
void			test_commands_from_tokens(t_token_list *tokens);
void			print_ast(t_ast *node, int level, t_env *env, t_shell *sh);

// syntax analysis
int				syntax_analysis(t_token_list *tokens);
int				check_pipe(t_token *prev, t_token *next);
int				check_redir(t_token *next);
int				check_logical(t_token *prev, t_token *token, t_token *next);
int				is_twochar(t_token *token);
int				check_paren(t_token *p, t_token *t, t_token *n, int *depth);
int				is_all_whitespace(const char *s);

// executor
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

// env manipulation
char			**env_list_to_array(t_env *env);
void			free_string_array(char **arr);

// last status
void			set_last_status(t_shell *shell, int status);
int				get_last_status(t_shell *shell);

// executor pipe
void			handle_left_child(int *fd, t_shell *shell, t_ast *node);
void			handle_right_child(int *fd, t_shell *shell, t_ast *node);

// builtin
void			builtin_exit(t_token_list *tokens, t_shell *shell);
int				parse_exit_code(char *arg_str, t_shell *shell);
int				builtin_pwd(t_env *env);
int				builtin_env(t_env *env);
int				builtin_echo(char **args);
int				builtin_cd(char **args, t_env *env);
int				builtin_export(char **args, t_env **env);
void			print_exported(t_env *env);
int				builtin_unset(char **args, t_env **env);

// setup_signals_prompt
void			setup_signals_prompt(void);
t_sig			setup_sigint_prompt(void);
void			handle_sigint_prompt(int sig);
void			actualize_sh_last_status(t_shell *sh, int new_value);

// setup_signals_heredoc
void			handle_sigeof_heredoc(char *delim);
void			handle_sigint_heredoc(int sig);
void			setup_signals_heredoc(void);

// setup_signals_exec
void			setup_signals_exec(void);
t_sig			setup_sigint_exec(void);
t_sig			setup_sigquit_exec(void);
void			handle_sigint_exec(int sig);
void			handle_sigquit_exec(int sig);

// prompt
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
