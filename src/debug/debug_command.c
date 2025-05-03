/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:20:41 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/03 18:56:05 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
void	print_command(t_command *cmd)
{
	int i = 0;

	printf("=== Comando ===\n");

	printf("Args (%d):\n", cmd->arg_count);
	while (cmd->args && cmd->args[i])
	{
		printf("  args[%d]: [%s]\n", i, cmd->args[i]);
		i++;
	}

	if (cmd->infile)
		printf("Infile: [%s]\n", cmd->infile);
	if (cmd->outfile)
		printf("Outfile: [%s]\n", cmd->outfile);
	if (cmd->appendfile)
		printf("Appendfile: [%s]\n", cmd->appendfile);
	if (cmd->heredoc)
	{
		printf("Heredoc: YES\n");
		printf("Delimiter: [%s]\n", cmd->delimiter);
		printf("Heredoc Path: [%s]\n", cmd->heredoc_path);
	}
	printf("Is builtin: %d\n", cmd->is_builtin);
	printf("==============\n");
}

void	test_commands_from_tokens(t_token_list *tokens)
{
	t_token	*current = tokens->head;

	while (current)
	{
		if (is_pipe_or_logical(current))
		{
			printf("=== Operador ===\n");
			if (current->type == PIPE)
				printf("Operador: PIPE\n");
			else if (current->type == LOGICAL_AND)
				printf("Operador: AND (&&)\n");
			else if (current->type == LOGICAL_OR)
				printf("Operador: OR (||)\n");
			else if (current->type == PAREN_OPEN)
				printf("Operador: PAREN OPEN (\n");
			else if (current->type == PAREN_CLOSE)
				printf("Operador: PAREN CLOSE )\n");
			printf("================\n");
			current = current->next;
		}
		else
		{
			// Chama make_command e atualiza o ponteiro
			t_command *cmd = make_command(&current);
			if (!cmd)
			{
				printf("Erro ao criar comando\n");
				return;
			}
			print_command(cmd);
			command_free(cmd);
		}
	}
}

