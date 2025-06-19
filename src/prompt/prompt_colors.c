/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_colors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:37:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/23 00:41:39 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Builds user@host section of prompt with color formatting.
// 1. Appends red color code.
// 2. Adds username and @ symbol.
// 3. Appends hostname.
// 4. Resets color.
void	build_user_host(char *dst, size_t *i, char *user, char *host)
{
	append_str(dst, i, C_RED);
	append_str(dst, i, user);
	append_char(dst, i, '@');
	append_str(dst, i, host);
	append_str(dst, i, C_RESET);
}

// Builds current directory section of prompt with color formatting.
// 1. Appends colon separator.
// 2. Adds yellow color code.
// 3. Appends directory path.
// 4. Resets color.
void	build_cwd(char *dst, size_t *i, char *cwd)
{
	append_char(dst, i, ':');
	append_str(dst, i, C_YELLOW);
	append_str(dst, i, cwd);
	append_str(dst, i, C_RESET);
}

// Appends string to destination buffer.
// 1. Copies source string to current position.
// 2. Updates position index.
void	append_str(char *dst, size_t *i, const char *src)
{
	size_t	len;

	len = ft_strlen(src);
	ft_memcpy(dst + *i, src, len);
	*i += len;
}

// Appends single character to destination buffer.
// 1. Stores character at current position.
// 2. Increments position index.
void	append_char(char *dst, size_t *i, char c)
{
	dst[(*i)++] = c;
}
