// minishell.h:
typedef struct sigaction	t_sig;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	LOGICAL_AND,
	LOGICAL_OR,
	PAREN_OPEN,
	PAREN_CLOSE,
}	t_token_type;

typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote;

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef enum s_redir_type
{
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	int				heredoc_fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char			**args;
	int				arg_count;
	t_redir			*redirs;
	int				heredoc_fd;
	int				is_builtin;
}	t_command;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_token_list
{
	t_token	*head;
	t_token	*tail;
	int		size;
}	t_token_list;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	t_command		*cmd;
}	t_ast;

typedef struct s_args
{
	char			*arg;
	struct s_args	*next;
}	t_args;

typedef struct s_expand
{
	t_args	**head;
	t_quote	state;
	t_env	*env;
	char	*prefix;
	char	*cur;
	int		i;
}	t_exp;

typedef struct s_wild
{
	char	**parts;
	int		have_start;
	int		have_end;
	int		full;
}	t_wild;

typedef struct s_shell
{
	t_env			*env;
	t_token_list	*tokens;
	t_ast			*ast;
	int				last_status;
}	t_shell;

typedef struct s_quote_ctx
{
	char	*str;
	char	*result;
	int		i;
	int		j;
	t_quote	state;
}	t_quote_ctx;

// main.c:
void	prompt(t_shell *shell);
void	minishell(char *input, t_shell *shell);
void	parser(t_token_list *tokens, t_shell *shell);
int		handle_all_white_spaces(char *input);
volatile int		g_signal_status;

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	if (argc > 1 || argv[1])
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (127);
	}
	shell.env = init_env(envp);
	shell.tokens = NULL;
	shell.ast = NULL;
	shell.last_status = 0;
	prompt(&shell);
	clean_all(NULL, NULL, &shell.env);
	return (0);
}

void	prompt(t_shell *shell)
{
	char	*input;
	char	*pmt;

	while (1)
	{
		pmt = make_prompt(shell);
		g_signal_status = -1;
		input = readline(pmt);
		free(pmt);
		if (g_signal_status == 130)
			update_sh_last_status(shell, -1);
		if (!input)
			break ;
		if (handle_all_white_spaces(input))
			continue ;
		if (ft_strcmp(input, "exit") == 0)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			free(input);
			break ;
		}
		add_history(input);
		minishell(input, shell);
		free(input);
	}
}

void	minishell(char *input, t_shell *shell)
{
	t_token_list	*tokens;

	tokens = tokenizer(input);
	if (!tokens)
	{
		shell->last_status = 2;
		return ;
	}
	shell->tokens = tokens;
	parser(tokens, shell);
}

void	parser(t_token_list *tokens, t_shell *shell)
{
	t_ast	*ast;

	if (!syntax_analysis(tokens))
	{
		token_list_free(tokens);
		shell->last_status = 2;
		return ;
	}
	ast = parse_logical(&tokens->head, shell);
	shell->ast = ast;
	if (g_signal_status != -3)
		executor(tokens, shell, ast);
	token_list_free(tokens);
	ast_free(shell->ast);
	shell->ast = NULL;
	shell->tokens = NULL;
}

int	handle_all_white_spaces(char *input)
{
	if (input[0] == '\0' || is_all_whitespace(input))
	{
		free(input);
		return (1);
	}
	return (0);
}

// expansion.c:
void	expander(char ***args, t_env *env, int *size_args, t_shell *sh)
{
	t_args	*expanded_envs;
	t_args	*head;
	char	**expanded;

	if (!(*args) || !(*args)[0])
		return ;
	expanded_envs = expand_env(*args, env, sh);
	head = expand_wild(expanded_envs);
	free_args_list(expanded_envs);
	expanded = list_to_args(head, size_args);
	free_args_temp(*args);
	(*args) = expanded;
	free_args_list(head);
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

// expand_env.c:
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

// expand_env_utils.c:
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

void	update_state_quote(t_quote *state, char c)
{
	if ((c == '\'' && *state == SINGLE_QUOTE)
		|| (c == '\"' && *state == DOUBLE_QUOTE))
		*state = NO_QUOTE;
	else if ((c == '\'' && *state == NO_QUOTE))
		*state = SINGLE_QUOTE;
	else if ((c == '\"' && *state == NO_QUOTE))
		*state = DOUBLE_QUOTE;
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

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
	{
		res = ft_strdup(s1);
		free(s1);
		return (res);
	}
	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

// expand_redir.c:
void	expand_redirs(t_redir *redirs, t_shell *sh)
{
	t_redir	*cur;

	cur = redirs;
	while (cur)
	{
		if (cur->type != R_HEREDOC)
			expand_redir(cur, sh);
		cur = cur->next;
	}
}

void	expand_redir(t_redir *redir, t_shell *sh)
{
	t_args	*arg_list;
	t_args	*expanded_list;
	int		count;

	arg_list = NULL;
	add_token(&arg_list, redir->filename);
	expanded_list = expand_redir_wild(arg_list);
	if (!expanded_list)
		count = 0;
	else
		count = count_args(expanded_list);
	if (count == 0)
		handle_no_match(sh, redir);
	else if (count > 1)
		handle_ambiguous(sh, redir);
	else
		handle_single_match(redir, expanded_list);
	free_args_list(arg_list);
	free_args_list(expanded_list);
}

t_args	*expand_redir_wild(t_args *envs)
{
	t_args	*result;
	t_args	*matches;
	t_wild	wild;

	result = NULL;
	while (envs)
	{
		if (is_wildcard(envs->arg))
		{
			wild = parse_pattern(envs->arg);
			matches = wild_matches(&wild);
			if (!matches)
			{
				free_array((void **)wild.parts);
				free_args_list(result);
				return (NULL);
			}
			sort_append(&result, matches);
			free_array((void **)wild.parts);
		}
		else
			add_token_free(&result, rem_quotes(envs->arg, 0));
		envs = envs->next;
	}
	return (result);
}

// expand_redir_utils.c:
int	count_args(t_args *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

void	handle_no_match(t_shell *sh, t_redir *redir)
{
	sh->last_status = 1;
	ft_putstr_fd("-minishell: ", STDERR_FILENO);
	ft_putstr_fd(redir->filename, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	redir->filename = NULL;
}

void	handle_ambiguous(t_shell *sh, t_redir *redir)
{
	sh->last_status = 1;
	ft_putstr_fd("-minishell: ", STDERR_FILENO);
	ft_putstr_fd(redir->filename, STDERR_FILENO);
	ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
	redir->filename = NULL;
}

void	handle_single_match(t_redir *redir, t_args *expanded_list)
{
	char	*old_filename;

	old_filename = redir->filename;
	redir->filename = ft_strdup(expanded_list->arg);
	free(old_filename);
}

// expand_token.c:
t_args	*expand_token(char *input, t_env *env, t_shell *sh)
{
	t_exp	exp;
	t_args	*result;

	init_expander(&exp, input, env);
	while (input[exp.i])
		handle_next_token(&exp, input, sh);
	if (exp.prefix)
		add_token(exp.head, exp.prefix);
	result = *exp.head;
	free(exp.head);
	if (exp.prefix)
		free (exp.prefix);
	return (result);
}

void	handle_next_token(t_exp *exp, char *input, t_shell *sh)
{
	char	*key;

	if (input[exp->i] == '$')
	{
		if (input[exp->i + 1] == '?')
		{
			expand_status(exp, sh->last_status);
			exp->i = exp->i + 2;
			return ;
		}
		if (handle_special_expand(exp, input))
			return ;
		key = extract_key(input, &exp->i);
		if (key)
		{
			expand_variable(exp, key);
			free(key);
		}
	}
	else
	{
		exp->cur = start_prefix(input, &exp->i, &exp->state);
		exp->prefix = ft_strjoin_free(exp->prefix, exp->cur);
		exp->cur = NULL;
	}
}

int	handle_special_expand(t_exp *exp, char *input)
{
	char	*temp;

	if (input[exp->i + 1] == '\0' || input[exp->i + 1] == ' '
		|| input[exp->i + 1] == '\"' || input[exp->i + 1] == '\''
		|| input[exp->i + 1] == '/')
	{
		exp->cur = ft_strdup("$");
		if (!exp->cur)
			return (1);
		if (!exp->prefix)
			exp->prefix = ft_strdup("");
		temp = exp->prefix;
		exp->prefix = ft_strjoin(temp, exp->cur);
		free(temp);
		free(exp->cur);
		exp->cur = NULL;
		exp->i++;
		return (1);
	}
	return (0);
}

void	init_expander(t_exp *exp, char *input, t_env *env)
{
	exp->i = 0;
	exp->state = NO_QUOTE;
	exp->head = (t_args **) malloc(sizeof(t_args *));
	*exp->head = NULL;
	exp->prefix = start_prefix(input, &exp->i, &exp->state);
	exp->cur = NULL;
	exp->env = env;
}

// expand_wild.c:
t_args	*expand_wild(t_args *envs)
{
	t_args	*result;
	t_args	*matches;
	t_wild	wild;

	result = NULL;
	while (envs)
	{
		if (is_wildcard(envs->arg))
		{
			wild = parse_pattern(envs->arg);
			matches = wild_matches(&wild);
			if (!matches)
				add_token_free(&result, rem_quotes(envs->arg, 0));
			else
				sort_append(&result, matches);
			free_array((void **) wild.parts);
		}
		else
			add_token_free(&result, rem_quotes(envs->arg, 0));
		envs = envs->next;
	}
	return (result);
}

t_wild	parse_pattern(char *arg)
{
	t_wild	wild;

	if (only_asterisk(arg))
		return (wild.parts = NULL, wild.full = 1, wild);
	wild.full = 0;
	wild.parts = split_wildcard(arg);
	wild.have_start = 0;
	wild.have_end = 0;
	if (arg && arg[0] != '\0' && arg[0] != '*')
		wild.have_start = 1;
	if (arg && ft_strlen(arg) > 0 && arg[ft_strlen(arg) - 1] != '*')
		wild.have_end = 1;
	if (!wild.parts || count_array((void **)wild.parts) == 0)
	{
		free(wild.parts);
		wild.parts = NULL;
	}
	return (wild);
}

t_args	*wild_matches(t_wild *wild)
{
	struct dirent	*entry;
	t_args			*matches;
	DIR				*dir;

	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (dir && entry)
	{
		if (entry->d_name[0] == '.'
			&& (!wild->parts || !wild->parts[0] || wild->parts[0][0] != '.'))
			;
		else if (match_pattern(entry->d_name, wild))
			add_token(&matches, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	return (matches);
}

int	is_wildcard(char *arg)
{
	t_quote	state;
	int		i;

	i = 0;
	state = NO_QUOTE;
	while (arg[i])
	{
		update_state_quote(&state, arg[i]);
		if (arg[i] == '*' && state == NO_QUOTE)
			return (1);
		i++;
	}
	return (0);
}

int	only_asterisk(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != '*')
			return (0);
		i++;
	}
	return (1);
}

// expand_wild_split.c:
void	init_vars_split_wildcard(t_quote *state, char ***parts, int *i, int *j)
{
	*state = NO_QUOTE;
	(*parts) = NULL;
	*i = 0;
	*j = 0;
}

char	**split_wildcard(char *input)
{
	t_quote	state;
	char	**parts;
	int		i;
	int		j;

	init_vars_split_wildcard(&state, &parts, &i, &j);
	while (input [i])
	{
		update_state_quote(&state, input[i]);
		if (input[i] == '*' && state == NO_QUOTE)
		{
			if (i - j > 0)
				add_token_to_array(&parts,
					rem_quotes(ft_substr(input, j, i - j), 1));
			j = i + 1;
		}
		i++;
	}
	if (i - j > 0)
		add_token_to_array(&parts, rem_quotes(ft_substr(input, j, i - j), 1));
	else if (j == 0)
		add_token_to_array(&parts, ft_strdup(""));
	return (parts);
}

void	add_token_to_array(char ***array, char *token)
{
	char	**new_array;
	int		len;
	int		i;

	if (!token)
		return ;
	len = count_array((void **) *array);
	new_array = malloc(sizeof(char *) * (len + 2));
	if (!new_array)
	{
		free(token);
		return ;
	}
	i = 0;
	while (*array && (*array)[i])
	{
		new_array[i] = (*array)[i];
		i++;
	}
	new_array[i++] = token;
	new_array[i] = NULL;
	free(*array);
	*array = new_array;
}

// expand_wild_utils.c:
int	match_middle(char *pos, t_wild *wild, int i, int limit)
{
	size_t	part_len;

	while (i < limit)
	{
		part_len = ft_strlen(wild->parts[i]);
		pos = ft_strnstr(pos, wild->parts[i], ft_strlen(pos));
		if (!pos)
			return (0);
		pos += part_len;
		i++;
	}
	return (1);
}

int	match_end(const char *filename, const char *end)
{
	size_t	file_len;
	size_t	end_len;

	file_len = ft_strlen(filename);
	end_len = ft_strlen(end);
	if (file_len < end_len)
		return (0);
	return (ft_strncmp(filename + file_len - end_len, end, end_len) == 0);
}

int	match_pattern(const char *filename, t_wild *wild)
{
	char	*pos;
	int		i;
	int		len;

	i = 0;
	if (wild->full)
		return (1);
	pos = (char *) filename;
	if (!filename || !wild->parts)
		return (0);
	len = count_array((void **) wild->parts);
	if (wild->have_start)
	{
		if (ft_strncmp(filename, wild->parts[0],
				ft_strlen(wild->parts[0])) != 0)
			return (0);
		pos = (char *) filename + ft_strlen(wild->parts[0]);
		i++;
	}
	if (!match_middle(pos, wild, i, len - wild->have_end))
		return (0);
	if (wild->have_end)
		return (match_end(filename, wild->parts[len - 1]));
	return (1);
}

int	handle_opening_quote(char *input, int *i, t_quote *state)
{
	if ((input[*i] == '\'' && *state == NO_QUOTE))
	{
		*state = SINGLE_QUOTE;
		(*i)++;
		return (1);
	}
	else if ((input[*i] == '\"' && *state == NO_QUOTE))
	{
		*state = DOUBLE_QUOTE;
		(*i)++;
		return (1);
	}
	return (0);
}

int	update_state_quote_i(char *input, int *i, t_quote *state)
{
	int	handled;

	handled = 0;
	while (input[*i] == '\'' || input[*i] == '\"')
	{
		if ((input[*i] == '\'' && *state == SINGLE_QUOTE)
			|| (input[*i] == '\"' && *state == DOUBLE_QUOTE))
		{
			(*i)++;
			*state = NO_QUOTE;
			handled = 1;
		}
		else if (input[*i] == '\'' || input[*i] == '\"')
		{
			if (handle_opening_quote(input, i, state))
				handled = 1;
			else
				break ;
		}
		else
			break ;
	}
	return (handled);
}

// expansion_utils.c:
void	add_token(t_args **head, char *value)
{
	t_args	*new;
	t_args	*temp;

	if (!value)
		return ;
	new = (t_args *)malloc(sizeof(t_args));
	if (!new)
		return ;
	new->arg = ft_strdup(value);
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

void	add_token_free(t_args **head, char *value)
{
	add_token(head, value);
	free(value);
}

void	append_list(t_args **head, t_args *list)
{
	t_args	*temp;

	if (!list)
		return ;
	if (!(*head))
	{
		*head = list;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = list;
}

int	count_array(void **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

// handle_quotes.c:
void	init_vars_remove_quotes(int *i, int *j, t_quote *state)
{
	*i = 0;
	*j = 0;
	*state = NO_QUOTE;
}

char	*rem_quotes(char *str, int free_str)
{
	char	*result;
	int		i;
	int		j;
	int		quote_handled;
	t_quote	state;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	init_vars_remove_quotes(&i, &j, &state);
	while (str[i])
	{
		quote_handled = update_state_quote_i(str, &i, &state);
		if (quote_handled)
			continue ;
		if (!((str[i] == '\'' || str[i] == '\"') && state == NO_QUOTE))
			result[j++] = str[i];
		if (str[i])
			i++;
	}
	if (free_str)
		free(str);
	result[j] = '\0';
	return (result);
}

// sort_wildcard.c:
void	swap_args(t_args *a, t_args *b)
{
	char	*temp;

	temp = a->arg;
	a->arg = b->arg;
	b->arg = temp;
}

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

int	ft_strcasecmp(const char *s1, const char *s2)
{
	unsigned char	c1;
	unsigned char	c2;

	while (*s1 || *s2)
	{
		c1 = ft_tolower((int) *s1);
		c2 = ft_tolower((int) *s2);
		if (c1 != c2)
			return (c1 - c2);
		s1++;
		s2++;
	}
	return (0);
}

void	sort_args_list(t_args *list)
{
	t_args	*cur;
	t_args	*next;
	int		swapped;

	if (!list)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		cur = list;
		while (cur && cur->next)
		{
			next = cur->next;
			if (ft_strcasecmp(cur->arg, next->arg) > 0)
			{
				swap_args(cur, next);
				swapped = 1;
			}
			cur = cur->next;
		}
	}
}

void	sort_append(t_args **head, t_args *list)
{
	if (!list)
		return ;
	sort_args_list(list);
	append_list(head, list);
}
