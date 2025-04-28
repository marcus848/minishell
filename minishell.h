/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:11:14 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/28 16:52:51 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/include/libft.h"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

// to use readline
# include <readline/readline.h>

// to use add_history
# include <readline/history.h>

typedef struct s_simple_cmd
{
	char	**args;
	int		arg_count;
}	t_simple_cmd;

typedef struct s_command
{
	t_simple_cmd	**simple_cmds;
	int				cmd_count;
	char			*infile;
	char			*outfile;
	char			*errfile;
	int				append_out;
	int				append_err;
	int				heredoc;
	char			*delimiter;
	int				background;
}	t_command;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_LOGICAL_AND,
	TOKEN_LOGICAL_OR,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE,
	TOKEN_ASTERISK
}	t_token_type;

typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote;

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

// tokenizer
void	tokenizer(char	*input);
int		handle_operators(char *input, int *i, t_token_list *tokens);

// token
t_token	*new_token(t_token_type type, char *value);

// token list
void	token_list_init(t_token_list *list);
void	token_list_append(t_token_list *list, t_token *token);

// init env
t_env	*init_env(char **envp);

// clean
void	clean_all(t_env *env);
void	env_free_all(t_env **head);
void	exit_perror(const char *msg);


void	expander(char **args, t_env *env);
char	*expander_expand(char *input, t_env *env);
void	actualize_state_quote(char *input, t_quote *state, int *i);
char	*envp_expand(char *key, t_env *env);
char	*extract_key(char *input);
int	get_expand_len(char *input, t_quote state);
char	*expand_env(char *input, t_env *env);
char	*expand_token(char *input, t_env *env, t_quote state);

// debug functions
void	print_env(t_env *env);
void	print_tokens(t_token_list *tokens);
void	print_token(char *str_type, t_token *token);
void	test_expander(t_env *env);

#endif
