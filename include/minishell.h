/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:11:14 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/19 21:06:08 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef struct s_command
{
	char			**args;
	int				arg_count;
	char			*infile;
	char			*outfile;
	char			*appendfile;
	int				heredoc;
	char			*delimiter;
	char			*heredoc_path;
	int				is_builtin;
}	t_command;

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
	int				last_status;
}	t_env;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	t_command		*cmd;
}	t_ast;

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

// clean
void			clean_all(t_token_list *tokens, t_ast *node, t_env *env);
void			env_free_all(t_env **head);
void			exit_perror(const char *msg);
void			report_unexpected_quotes(const char token_value);
int				report_unexpected(const char *token_value);

// clean_ast
void			free_args(t_command *command);
void			command_free(t_command *command);
void			ast_free(t_ast *root);

// expansion
void			expander(char ***args, t_env *env);
char			*expander_expand(char *input, t_env *env);
char			*expand_token(char *input, t_env *env, t_quote state);

// expansion_types
char			*expand_env(char *key, t_env *env);

// expansion_utils
void			update_state_quote(char *input, t_quote *state, int *i);
char			*extract_key(char *input);
int				get_expand_len(char *input, t_quote state);

// ast
t_ast			*parse_command(t_token **token);
t_ast			*parse_logical(t_token **token);
t_ast			*parse_pipe(t_token **token);
t_ast			*parse_simple_command(t_token **token);
t_ast			*parse_subshell(t_token **token);

// commands
t_command		*make_command(t_token **token);
t_command		*init_command(void);
void			parse_redirect(t_token **token, t_command **command);
void			parse_heredoc(t_token **token, t_command **command);
char			**get_args(t_token **token, int size_args);

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
void			print_ast(t_ast *node, int level);

// syntax analysis
int				syntax_analysis(t_token_list *tokens);
int				check_pipe(t_token *prev, t_token *next);
int				check_redir(t_token *next);
int				check_logical(t_token *prev, t_token *token, t_token *next);
int				is_twochar(t_token *token);
int				check_paren(t_token *p, t_token *t, t_token *n, int *depth);

// executor
void			executor(t_token_list *tokens, t_ast *node, t_env *env);
void			handle_redirections(t_command *cmd);
void			apply_input_redir(t_command *cmd);
void			apply_output_redir(t_command *cmd);
int				save_fds(int *save_stdin, int *save_stdout);
int				restore_fds(int save_stdin, int save_stdout);
char			**env_list_to_array(t_env *env);
void			free_string_array(char **arr);
void			set_last_status(t_env *env, int status);
int				get_last_status(t_env *env);
int				exec_dispatch(char **args, t_env *env, char **envp);
void			execve_with_path(char **args, t_env *env, char **envp);
void			try_exec_explicit(char *cmd, char **args, char **envp);
int				is_builtin(char *cmd);
int				run_builtin(char **args, t_env *env);
int				is_executable_command(char *cmd, t_env *env);
int				is_explicit_executable(char *cmd);
int				search_in_path(char *cmd, t_env *env);
int				try_exit_builtin(t_token_list *toks, t_ast *node, t_env *env);
int				try_other_builtin(char **args, t_env *env);
void			run_external_command(char **args, t_command *cmd, t_env *env);

// builtin
void			builtin_exit(t_token_list *tokens, t_ast *node, t_env *env);
int				parse_exit_code(char *arg_str, t_env *env);
int				builtin_pwd(t_env *env);
int				builtin_env(t_env *env);
int				builtin_echo(char **args);
int				builtin_cd(char **args, t_env *env);

#endif
