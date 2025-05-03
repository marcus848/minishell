/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:42:03 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/03 19:06:30 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	command_free(t_command *command)
{
	if (command->args)
		free_args(command);
	if (command->infile)
		free(command->infile);
	if (command->outfile)
		free(command->outfile);
	if (command->appendfile)
		free(command->appendfile);
	if (command->heredoc_path)
		free(command->heredoc_path);
	if (command->delimiter)
		free(command->delimiter);
	free(command);
}


void	free_args(t_command *command)
{
	int	i;

	i = 0;
	while (i < command->arg_count)
		free(command->args[i++]);
	free(command->args);
}
