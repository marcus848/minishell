/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 04:20:19 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/29 04:23:06 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_args(t_args *head);

// Build a NULL-terminated argv-line array from one input line, running the
// expander on it.
char	**expand_line_to_words(char *line, t_env *env, t_shell *shell)
{
	t_args	*head;
	t_args	*iter;
	char	**words;
	int		count;
	int		i;

	head = expand_token(line, env, shell);
	count = count_args(head);
	words = (char **)malloc(sizeof(char *) * (count + 1));
	if (!words)
		exit_perror("malloc");
	iter = head;
	i = 0;
	while (iter)
	{
		words[i++] = ft_strdup(iter->arg);
		iter = iter->next;
	}
	words[i] = NULL;
	free_args_list(head);
	return (words);
}

int	count_args(t_args *head)
{
	int	count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

// Join an argv-like back into one string with spaces.
char	*join_words(char **words)
{
	char	*out;
	char	*tmp;
	int		count;
	int		i;

	count = count_array((void **)words);
	out = ft_strdup("");
	if (!out)
		exit_perror("ft_strdup");
	i = 0;
	while (i < count)
	{
		tmp = ft_strjoin(out, words[i]);
		free(out);
		out = tmp;
		if (i + 1 < count)
		{
			tmp = ft_strjoin(out, " ");
			free(out);
			out = tmp;
		}
		i++;
	}
	return (out);
}
