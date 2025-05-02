/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:29:45 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/02 15:12:06 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Print one bash-style syntax error for quotes
void	report_unexpected_quotes(const char token_value)
{
	write(2, "-minishell: syntax error near unexpected token `", 49);
	write(2, &token_value, 1);
	write(2, "'\n", 2);
}

// Print one bash-style syntax error and return 0
int	report_unexpected(const char *token_value)
{
	write(2, "-minishell: syntax error near unexpected token `", 49);
	write(2, token_value, ft_strlen(token_value));
	write(2, "'\n", 2);
	return (0);
}
