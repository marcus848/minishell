/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 00:01:06 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/29 04:24:07 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		process_heredoc(char *delim, int no_expand, t_env *env, t_shell *sh);
char	*get_expanded(char *line, int no_expand, t_env *env, t_shell *sh);

// Recursively prepare all heredocs in the AST before forking, so readline()
// runs in the parent shell.
void	prepare_heredocs(t_ast *node, t_shell *sh)
{
	int	no_expand;

	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->cmd->heredoc)
	{
		no_expand = node->cmd->quoted_delim;
		node->cmd->heredoc_fd = process_heredoc(node->cmd->delimiter,
				no_expand, sh->env, sh);
	}
	prepare_heredocs(node->left, sh);
	prepare_heredocs(node->right, sh);
}

// Read lines until "DELIMITER", expand each, write into a pipe, then return
// the read-end fd for dup2().
int	process_heredoc(char *delim, int no_expand, t_env *env, t_shell *sh)
{
	int		fds[2];
	char	*line;
	char	*expanded;

	if (pipe(fds) < 0)
		exit_perror("pipe");
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		expanded = get_expanded(line, no_expand, env, sh);
		write(fds[1], expanded, ft_strlen(expanded));
		write(fds[1], "\n", 1);
		free(expanded);
		free(line);
	}
	close(fds[1]);
	return (fds[0]);
}

char	*get_expanded(char *line, int no_expand, t_env *env, t_shell *sh)
{
	char	**words;
	char	*res;

	if (no_expand)
		res = ft_strdup(line);
	else
	{
		words = expand_line_to_words(line, env, sh);
		res = join_words(words);
		free_args_temp(words);
	}
	return (res);
}
