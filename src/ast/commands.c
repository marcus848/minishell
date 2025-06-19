/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:58:35 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/10 00:36:55 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Constructs a t_command node from tokens.
// 1. Initializes command structure.
// 2. Allocates space for arguments.
// 3. Copies WORD tokens into args array.
// 4. Handles redirections via parse_redirect().
// 5. Ends args with NULL terminator.
// 6. Updates token pointer to next control operator.
// Note: Relies on get_size_args for pre-counting arguments.
t_command	*make_command(t_token **token, t_shell *shell)
{
	t_command	*cmd;
	t_token		*t;
	int			arg;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	cmd->arg_count = get_size_args(token);
	cmd->args = (char **) malloc(sizeof(char *) * (cmd->arg_count + 1));
	if (!cmd->args)
		return (NULL);
	t = *token;
	arg = 0;
	while (t && !is_pipe_or_logical(t))
	{
		if (t->type == WORD)
		{
			cmd->args[arg++] = ft_strdup(t->value);
			t = t->next;
		}
		else if (is_redirect(t))
			parse_redirect(&t, &cmd, shell);
	}
	cmd->args[arg] = NULL;
	return ((*token) = t, cmd);
}

// Initializes and returns a blank t_command struct.
// 1. Sets args and redirs to NULL.
// 2. Sets heredoc_fd to -1.
t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = (t_command *) malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->arg_count = 0;
	cmd->redirs = NULL;
	cmd->heredoc_fd = -1;
	cmd->is_builtin = 0;
	return (cmd);
}

// Parses and appends a redirection to the command.
// 1. Checks if next token exists.
// 2. Handles HEREDOC using parse_heredoc.
// 3. Maps REDIR tokens to enum types.
// 4. Appends redirection and advances token pointer.
// Note: Skips two tokens for regular redirections (operator + file).
void	parse_redirect(t_token **token, t_command **cmd, t_shell *sh)
{
	t_redir_type	type;

	type = -1;
	if (!(*token)->next)
	{
		*token = (*token)->next;
		return ;
	}
	if ((*token)->type == HEREDOC)
		parse_heredoc(token, cmd, sh);
	else
	{
		if ((*token)->type == REDIR_IN)
			type = R_IN;
		else if ((*token)->type == REDIR_OUT)
			type = R_OUT;
		else if ((*token)->type == REDIR_APPEND)
			type = R_APPEND;
		append_redir(&(*cmd)->redirs, type, (*token)->next->value);
		*token = (*token)->next->next;
	}
}
