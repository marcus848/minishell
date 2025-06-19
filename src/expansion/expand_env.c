/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:22:33 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/01 16:21:42 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Expands environment variables from a string array of arguments.
// 1. Starts expansion with the first valid token.
// 2. Concatenates all expanded tokens into a linked list (t_args).
// 3. Returns the list of expanded arguments.
t_args	*expand_env(char **args, t_env *env, t_shell *sh)
{
	t_args	*head;
	t_args	*cur;
	t_args	*new;
	int		i;

	if (!args || !args[0])
		return (NULL);
	head = expand_token(args[0], env, sh);
	i = 1;
	while (!head && args[i])
		head = expand_token(args[i++], env, sh);
	cur = head;
	while (args[i])
	{
		new = expand_token(args[i], env, sh);
		while (cur->next)
			cur = cur->next;
		cur->next = new;
		i++;
	}
	return (head);
}

// Extracts the prefix (before '$') from input, respecting quote context.
// 1. Scans input until '$' or null terminator (except inside single quotes).
// 2. Builds prefix string character-by-character.
// 3. Updates quote state for each character scanned.
char	*start_prefix(char *input, int *i, t_quote *state)
{
	char	*prefix;
	char	*temp;
	char	*new;

	prefix = NULL;
	new = NULL;
	while (input[*i])
	{
		update_state_quote(state, input[*i]);
		if (input[*i] == '\0' || (input[*i] == '$' && *state != SINGLE_QUOTE))
			break ;
		temp = ft_substr(input, *i, 1);
		if (prefix)
		{
			new = ft_strjoin(prefix, temp);
			free(prefix);
			free(temp);
			prefix = new;
		}
		else
			prefix = temp;
		(*i)++;
	}
	return (prefix);
}

// Expands a variable key and appends its value to the current prefix.
// 1. Looks up the variable in the environment.
// 2. Skips empty values.
// 3. Appends the value to 'exp->prefix'.
// 4. If not quoted, splits and adds tokens individually.
void	expand_variable(t_exp *exp, char *key)
{
	char	*temp;

	exp->cur = find_env_value(key, exp->env);
	if (!exp->cur)
		return ;
	if (ft_strcmp(exp->cur, "") == 0)
	{
		free(exp->cur);
		return ;
	}
	if (!exp->prefix)
		exp->prefix = ft_strdup("");
	temp = exp->prefix;
	exp->prefix = ft_strjoin(temp, exp->cur);
	free(temp);
	free(exp->cur);
	exp->cur = NULL;
	if (exp->state == NO_QUOTE)
		handle_no_quotes(exp);
}

// Expands the special variable '$?' using the last exit status.
// 1. Converts status to string.
// 2. Appends it to 'exp->prefix'.
void	expand_status(t_exp *exp, int status)
{
	char	*temp;

	exp->cur = ft_itoa(status);
	if (!exp->cur)
		return ;
	if (!exp->prefix)
		exp->prefix = ft_strdup("");
	temp = exp->prefix;
	exp->prefix = ft_strjoin(temp, exp->cur);
	free(temp);
	free(exp->cur);
	exp->cur = NULL;
}

// Handles variable expansion with no quotes (e.g., '$VAR').
// 1. Splits prefix by spaces.
// 2. If multiple parts, adds all but the last as individual args.
// 3. The last part is kept as prefix for continued parsing.
void	handle_no_quotes(t_exp *exp)
{
	char	**parts;
	int		i;

	parts = ft_split(exp->prefix, ' ');
	if (exp->prefix)
		free(exp->prefix);
	exp->prefix = NULL;
	if (count_array((void **) parts) == 1)
		exp->prefix = ft_strdup(parts[0]);
	else
	{
		i = 0;
		while (i == 0 || (parts[i] && parts[i + 1]))
			add_token(exp->head, parts[i++]);
		if (!parts[i])
			exp->prefix = NULL;
		else
			exp->prefix = ft_strdup(parts[i]);
	}
	free_args_temp(parts);
}
