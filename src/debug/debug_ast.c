/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:59:37 by marcudos          #+#    #+#             */
/*   Updated: 2025/04/30 19:19:38 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	printf("args: [[%s]", args[i]);
	i++;
	while (args[i])
	{
		printf(", [%s]", args[i]);
		i++;
	}
	printf(", [(NULL)]]");
	printf("\n");
}

void	print_node_type(t_node_type type)
{
	if (type == NODE_COMMAND)
		printf("[COMMAND]");
	else if (type == NODE_PIPE)
		printf("[PIPE]");
	else if (type == NODE_AND)
		printf("[AND]");
	else if (type == NODE_OR)
		printf("[OR]");
	else if (type == NODE_SUBSHELL)
		printf("[SUBSHELL]");
	else
		printf("[UNKNOWN]");
}

void	print_ast(t_ast *node, int level)
{
	if (!node)
		return ;
	
	for (int i = 0; i < level; i++)
		printf("    ");

	print_node_type(node->type);

	if (node->type == NODE_COMMAND && node->cmd && node->cmd->args)
	{
		printf(" ");
		print_args(node->cmd->args);
	}
	else
		printf("\n");

	if (node->left)
		print_ast(node->left, level + 1);
	if (node->right)
		print_ast(node->right, level + 1);

}

