/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_give_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:45:59 by sael-kha          #+#    #+#             */
/*   Updated: 2025/05/31 20:30:15 by sael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ms	*handle_special_env(int *s, int *e, char *str)
{
	char	*tmp;
	t_ms	*token;

	(*e)++;
	tmp = ft_substr(str, *s, *e);
	token = make_list(tmp, TOKEN_ENV);
	free(tmp);
	return (token);
}

static t_ms	*handle_env_var(int *s, int *e, char *str)
{
	char	*tmp;
	t_ms	*token;

	while (str[*s + *e] && (str[*s + *e] == '_' || ft_isalnum(str[*s + *e])))
		(*e)++;
	tmp = ft_substr(str, *s, *e);
	token = make_list(tmp, TOKEN_ENV);
	free(tmp);
	return (token);
}

static t_ms	*handle_word(int *s, int *e, char *str)
{
	char	*tmp;
	t_ms	*token;

	while (str[*s + *e] && str[*s + *e] != '$')
		(*e)++;
	tmp = ft_substr(str, *s, *e);
	*s += *e;
	token = make_list(tmp, TOKEN_WORD);
	free(tmp);
	return (token);
}

t_ms	*process_token(int *s, int *e, char *str, t_ms **head)
{
	t_ms	*current;

	current = NULL;
	if (str[*s] == '$')
	{
		(*s)++;
		if (ft_isdigit(str[*s]) || str[*s] == '?')
			current = handle_special_env(s, e, str);
		else if (str[*s] == '_' || ft_isalpha(str[*s]))
			current = handle_env_var(s, e, str);
		else
			current = make_list("$", TOKEN_WORD);
		*s += *e;
	}
	else
		current = handle_word(s, e, str);
	if (!*head)
		*head = current;
	else if (current)
		link_list(*head, current);
	*e = 0;
	return (*head);
}
