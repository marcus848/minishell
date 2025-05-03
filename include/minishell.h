/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:11:14 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/03 19:01:37 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

// to use readline
# include <readline/readline.h>

// to use add_history
# include <readline/history.h>

// MACRO FOR SYNTAX ERROR
# define ERR_QUOTE "minishell: syntax error: expecting closing quote"

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
	ASTERISK
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
}	t_env;

typedef struct s_ast
{
	t_node_type	type;
	struct s_ast	*left;
	struct s_ast	*right;
	char		**args;
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

// init env
t_env			*init_env(char **envp);

// clean
void			clean_all(t_env *env);
void			env_free_all(t_env **head);
void			exit_perror(const char *msg);
void			report_syntax_error(const char *msg);

// clean_ast
void			free_args(t_command *command);
void			command_free(t_command *command);

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
void			print_tokens(t_token_list *tokens);
void			print_token(char *str_type, t_token *token);
void			test_expander(t_env *env);
void			test_commands_from_tokens(t_token_list *tokens);

#endif
