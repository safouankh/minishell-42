/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mk_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:44:40 by sael-kha          #+#    #+#             */
/*   Updated: 2025/06/02 15:13:29 by sael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ms	*make_list(char *value, int type, ...)
{
	t_ms	*ms;

	ms = malloc(sizeof(t_ms));
	if (!ms)
	{
		perror("Error : ");
		exit(1);
	}
	ms->value = ft_strdup(value);
	ms->type = type;
	ms->next = NULL;
	return (ms);
}

void	link_list(t_ms *ms1, t_ms *ms2)
{
	while(ms1->next)
		ms1 = ms1->next;
	ms1->next = ms2;
}

int is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	GIRV(t_ms *head, t_env *env)
{
	char	*tmp;

	while (head)
	{
		if (head->type == TOKEN_WORD || head->type == TOKEN_DQUOTE)
		{
			tmp = head->value;
			head->value = shorting_code(head->value, env);
			free(tmp);
		}
		head = head->next;
	}
}

t_ms	*cut_word(char *input, int *i)
{
	char	c;
	char	*word;
	t_ms	*node;
	int		start;

	c = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != c)
		(*i)++;
	if (!input[*i])
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (NULL);
	}
	word = ft_substr(input, start, *i - start);
	if (c == '\'')
		node = make_list(word, TOKEN_SQUOTE);
	else
		node = make_list(word, TOKEN_DQUOTE);
	(*i)++;
	free(word);
	return (node);
}

static t_ms	*handle_special_tokens(char *input, int *i)
{
	if (input[*i] == '|')
		return (make_list("|", TOKEN_PIPE, (*i)++));
	else if (input[*i] == '>' && input[*i + 1] == '>')
		return (make_list(">>", TOKEN_APPEND, (*i) += 2));
	else if (input[*i] == '>')
		return (make_list(">", TOKEN_REDIR_OUT, (*i)++));
	else if (input[*i] == '<' && input[*i + 1] == '<')
		return (make_list("<<", TOKEN_HEREDOC, (*i) += 2));
	else if (input[*i] == '<')
		return (make_list("<", TOKEN_REDIR_IN, (*i)++));
	return (NULL);
}

static t_ms	*handle_word_token(char *input, int *i, int start)
{
	char	*word;
	t_ms	*token;

	while (input[*i] && !is_whitespace(input[*i])
		&& !ft_strchr("|<>\"\'", input[*i]))
		(*i)++;
	word = ft_substr(input, start, *i - start);
	token = make_list(word, TOKEN_WORD);
	free(word);	
	return (token);
}

static t_ms	*handel_tokens(char *input, int *i)
{
	t_ms	*current;
	int		start;

	while (is_whitespace(input[*i]))
		(*i)++;
	if (!input[*i])
		return (make_list("", TOKEN_EOF));
	start = *i;
	current = handle_special_tokens(input, i);
	if (current)
		return (current);
	if (input[*i] == '\'' || input[*i] == '\"')
		return (cut_word(input, i));
	return (handle_word_token(input, i, start));
}

t_command	*token_input(char *input, t_env *env)
{
	t_ms		*head;
	t_ms		*current;
	t_command	*com;
	int			i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		current = handel_tokens(input, &i);
		if (!current)
			return (ft_free(head), NULL);
		if (!head)
			head = current;
		else
			link_list(head, current);
	}
	if (head && !check_sintax(head))
		return (ft_free(head), NULL);
	GIRV(head, env);
	com = mk_command(head, NULL);
	ft_free(head);
	return (com);
}
