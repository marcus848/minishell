/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 00:51:34 by caide-so          #+#    #+#             */
/*   Updated: 2025/06/10 00:51:52 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_infiles(t_redir *head, t_shell *shell)
{
	int	fd;

	while (head)
	{
		fd = open(head->filename, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("-minishell: ", STDERR_FILENO);
			ft_putstr_fd(head->filename, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			shell->last_status = 1;
			return (-1);
		}
		close(fd);
		head = head->next;
	}
	return (0);
}

char	*get_last_infile(t_redir *head)
{
	char	*last_filename;

	last_filename = NULL;
	while (head)
	{
		last_filename = head->filename;
		head = head->next;
	}
	return (last_filename);
}

int	validate_out_append(t_redir *head, int flags)
{
	int	fd;

	while (head)
	{
		fd = open(head->filename, flags, 0644);
		if (fd < 0)
			exit_perror("open redirect");
		close(fd);
		head = head->next;
	}
	return (0);
}

char	*get_last_out_append(t_redir *head)
{
	char	*last_filename;

	last_filename = NULL;
	while (head)
	{
		last_filename = head->filename;
		head = head->next;
	}
	return (last_filename);
}
