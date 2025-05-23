/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:44:54 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/23 00:48:11 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*assemble_prompt(char *user, char *host, char *cwd, size_t len);

char	*make_prompt(void)
{
	char	*user;
	char	*host;
	char	*cwd;
	size_t	len;
	char	*prompt;

	user = get_user();
	host = read_hostname_file();
	cwd = get_cwd_display();
	len = ft_strlen(user) + 1 + ft_strlen(host) + 1 + ft_strlen(cwd) + 3;
	prompt = assemble_prompt(user, host, cwd, len);
	return (prompt);
}

char	*assemble_prompt(char *user, char *host, char *cwd, size_t len)
{
	char	*prompt;
	size_t	extra;
	size_t	i;

	extra = ft_strlen(C_RED) + ft_strlen(C_RESET) + ft_strlen(C_YELLOW)
		+ ft_strlen(C_RESET);
	prompt = (char *)malloc(len + extra + 1);
	i = 0;
	build_user_host(prompt, &i, user, host);
	build_cwd(prompt, &i, cwd);
	append_char(prompt, &i, '$');
	append_char(prompt, &i, ' ');
	append_char(prompt, &i, '\0');
	free(host);
	free(cwd);
	return (prompt);
}
