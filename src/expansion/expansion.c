/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:14:44 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/19 11:34:08 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_args	*args_to_list(char **args);
char	**list_to_args(t_args *args, int *size_args);
t_args	*expand_token(char *input, t_env *env);

void	add_token(t_args **head, char *value)
{
	t_args	*new;
	t_args	*temp;

	if (!value)
		return ;
	new = (t_args *)malloc(sizeof(t_args));
	if (!new)
		return ;
	new->arg = value;
	new->next = NULL;
	if (!(*head))
		*head = new;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

char	*extract_key(char *input, int *i)
{
	int		start;

	if (input[*i] != '$')
		return (NULL);
	(*i)++;
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

void	expander(char ***args, t_env *env, int *size_args)
{
	t_args	*head;
	// t_args	*cur;
	// t_args	*new;
	char		**expanded;
	// int		i;

	if (!(*args) || !(*args)[0])
		return ;
	head = expand_env(*args, env);
	expanded = list_to_args(head, size_args);
	free_args_temp(*args);
	(*args) = expanded;
	free_args_list(head);
}

void	update_state_quote(char *input, int *i, t_quote *state)
{
	while (input[*i] == '\'' || input[*i] == '\"')
	{
		if ((input[*i] == '\'' && *state == SINGLE_QUOTE)
			|| (input[*i] == '\"' && *state == DOUBLE_QUOTE))
		{
			(*i)++;
			*state = NO_QUOTE;
		}
		else if ((input[*i] == '\'' && *state == NO_QUOTE))
		{
			*state = SINGLE_QUOTE;
			(*i)++;
		}
		else if ((input[*i] == '\"' && *state == NO_QUOTE))
		{
			*state = DOUBLE_QUOTE;
			(*i)++;
		}
		else
			break ;
	}
}

char	*find_env_value(char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(key, env->key) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

void	expand_variable(t_exp *exp, char *key)
{
	char	*temp;
	char	**parts;
	int	i;

	exp->cur = find_env_value(key, exp->env);
	free(key);
	if (!exp->cur)
		return ;
	if (!exp->prefix)
		exp->prefix = ft_strdup("");
	temp = exp->prefix;
	exp->prefix = ft_strjoin(temp, exp->cur);
	free(temp);
	free(exp->cur);
	exp->cur = NULL;
	if (exp->state == NO_QUOTE)
	{
		parts = ft_split(exp->prefix, ' ');
		i = 0;
		while (i == 0 || (parts[i] && parts[i + 1]))
			add_token(exp->head, parts[i++]);
		if (!parts[i])
			exp->prefix = NULL;
		else
			exp->prefix = ft_strdup(parts[i]);
		free(parts);
	}
}

char	*start_prefix(char *input, int *i, t_quote *state)
{
	char	*prefix;
	char	*temp;
	char	*new;

	prefix = NULL;
	while (input[*i])
	{
		update_state_quote(input, i, state);
		if (input[*i] == '$' && *state != SINGLE_QUOTE)
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

t_args	*expand_token(char *input, t_env *env)
{
	char	*key;
	int	i;
	t_exp	exp;

	i = 0;
	exp.state = NO_QUOTE; 
	exp.head = NULL;
	exp.env = env;
	exp.head = (t_args **) malloc(sizeof(t_args *));
	*exp.head = NULL;
	exp.prefix = NULL;
	exp.prefix = start_prefix(input, &i, &exp.state);
	while (input[i])
	{
		if (input[i] == '$')
		{
			key = extract_key(input, &i);
			if (key)
				expand_variable(&exp, key);
		}
		else
		{
			exp.cur = start_prefix(input, &i, &exp.state);
			exp.prefix = ft_strjoin(exp.prefix, exp.cur);
			exp.cur = NULL;
		}
	}
	add_token(exp.head, exp.prefix);
	return ((*exp.head));
}

char	**list_to_args(t_args *args, int *size_args)
{
	t_args	*temp;
	char	**res;
	int		size;
	int		i;

	temp = args;
	size = 0;
	while (temp)
	{
		size++;
		temp = temp->next;
	}
	*size_args = size;
	res = (char **) malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (args)
	{
		res[i] = ft_strdup(args->arg);
		i++;
		args = args->next;
	}
	res[i] = NULL;
	return (res);
}

t_args	*args_to_list(char **args)
{
	t_args	*head;
	t_args	*arg;
	t_args	*new;
	int		i;

	i = 0;
	while (args[i])
	{
		if (i == 0)
		{
			head = (t_args *) malloc(sizeof(t_args));
			head->arg = ft_strdup(args[i]);
			head->next = NULL;
			arg = head;
		}
		else
		{
			new = (t_args *) malloc(sizeof(t_args));
			new->arg = ft_strdup(args[i]);
			new->next = NULL;
			arg->next = new;
			arg = arg->next;
		}
		i++;
	}
	return (head);
}


