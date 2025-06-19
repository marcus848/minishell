/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 00:01:06 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/06 11:15:52 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_expanded(char *line, int no_expand, t_env *env, t_shell *sh);
char	*heredoc_expand_vars(char *line, t_env *env, int last_status);
void	write_and_free_line(int fd, char *line, char *expanded);
int		handle_end_heredoc(char *line, char *delim);

// Processes heredoc input until delimiter.
// 1. Creates pipe for heredoc output.
// 2. Reads lines with expansion handling.
// 3. Writes results to pipe.
// Returns read-end file descriptor.
int	process_hd(char *delim, int no_expand, t_env *env, t_shell *sh)
{
	int		fds[2];
	char	*line;
	char	*expanded;

	if (pipe(fds) < 0)
		exit_perror("pipe");
	setup_signals_heredoc();
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (g_signal_status == 130 && (update_sh_last_status(sh, -3), 1))
			break ;
		if (handle_end_heredoc(line, delim))
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		expanded = get_expanded(line, no_expand, env, sh);
		write_and_free_line(fds[1], line, expanded);
	}
	enable_echoctl();
	close(fds[1]);
	return (fds[0]);
}

// Checks for heredoc termination conditions.
// 1. Matches exact delimiter.
// 2. Handles EOF without newline.
// 3. Handles NULL input (EOF).
// Returns 1 if heredoc should end, 0 otherwise.
int	handle_end_heredoc(char *line, char *delim)
{
	if (line && ft_strncmp(line, delim, ft_strlen(delim)) == 0)
	{
		free(line);
		return (1);
	}
	else if (line && line[ft_strlen(line) - 1] != '\n')
	{
		handle_sigeof_heredoc(delim);
		free(line);
		return (1);
	}
	else if (!line)
	{
		handle_sigeof_heredoc(delim);
		return (1);
	}
	return (0);
}

// Writes expanded line to pipe and frees memory.
// 1. Writes expanded content.
// 2. Adds newline.
// 3. Frees both original and expanded strings.
void	write_and_free_line(int fd, char *line, char *expanded)
{
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
	free(line);
}

// Expands variables in heredoc content if enabled.
// 1. Returns direct copy if expansion disabled.
// 2. Otherwise performs variable expansion.
// Returns expanded string.
char	*get_expanded(char *line, int no_expand, t_env *env, t_shell *sh)
{
	char	*res;

	if (no_expand)
		res = ft_strdup(line);
	else
		res = heredoc_expand_vars(line, env, sh->last_status);
	return (res);
}

// Expands variables in heredoc content.
// 1. Scans for $ variables.
// 2. Expands each variable found.
// 3. Preserves other characters literally.
// Returns expanded string.
char	*heredoc_expand_vars(char *line, t_env *env, int last_status)
{
	char	*out;
	char	*val;
	int		i;

	out = ft_strdup("");
	if (!out)
		exit_perror("ft_strdup");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			val = get_var_value(line, &i, env, last_status);
			out = ft_strjoin_free(out, val);
			continue ;
			out = ft_strjoin_free(out, val);
			continue ;
		}
		out = ft_strjoin_char_free(out, line[i]);
		i++;
	}
	return (out);
}
