/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mk_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:44:40 by sael-kha          #+#    #+#             */
/*   Updated: 2025/05/13 20:31:54 by sael-kha         ###   ########.fr       */
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
	if (input[*i] == '\0')
		return (ft_putstr_fd("Minishell: error syntax\n", 2), NULL);
	word = ft_substr(input, start, *i - start);
	if (c == '\'')
		node = make_list(word, TOKEN_SQUOTE);
	else
		node = make_list(word, TOKEN_DQUOTE);
	(*i)++;
	free(word);
	return (node);
}

t_command	*token_input(char *input)
{
	t_ms	*head;
	t_ms	*current;
	int		start;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while(input[i])
	{
		while (is_whitespace(input[i]))
			i++;
		if (!input[i])
			break;
		start = i;
		if (input[i] == '|')
			current = make_list("|", TOKEN_PIPE, i++);
		else if (input[i] == '>' && input[i + 1] == '>')
			current = make_list(">>", TOKEN_APPEND, i += 2);
		else if (input[i] == '>')
			current = make_list(">", TOKEN_REDIR_OUT, i++);
		else if (input[i] == '<' && input[i + 1] == '<')
			current = make_list("<<", TOKEN_HEREDOC, i += 2);
		else if (input[i] == '<')
			current = make_list("<", TOKEN_REDIR_IN, i++);
		else if (input[i] == '\'' || input[i] == '\"')
			current = cut_word(input, &i);
		else
		{
			while (input[i] && !is_whitespace(input[i]) &&
				!ft_strchr("|<>\"\'", input[i]))
				i++;
			char *test = ft_substr(input, start, i - start);
			current = make_list(test, TOKEN_WORD);
			free(test);
		}
		if (!current)
			return (NULL);
		if (!head)
			head = current;
		else
			link_list(head, current);
	}
	return (mk_command(head));
}
