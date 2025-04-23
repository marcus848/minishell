/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:11:14 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/22 22:46:16 by caide-so         ###   ########.fr       */
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
	TOKEN_AMPERSAND,
	TOKEN_LOGICAL_AND,
	TOKEN_LOGICAL_OR,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE
}	t_token_type;

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

#endif
