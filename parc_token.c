/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parc_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 18:23:20 by sael-kha          #+#    #+#             */
/*   Updated: 2025/05/19 20:24:53 by sael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	commands(t_ms *head)
{
	int	i;

	i = 0;
	while (head->type == TOKEN_WORD ||
		head->type == TOKEN_SQUOTE || head->type == TOKEN_DQUOTE)
	{
		i++;
		head = head->next;
		if (!head)
			break ;
	}
	return (i);
}

t_ms	*re_pipe(t_ms *head)
{
	while (head && head->type != TOKEN_PIPE)
		head = head->next;
	return (head);
}

t_redirection	*mk_redirection(t_ms *head)
{
	t_redirection	*redirection;

	if (!head)
		return NULL;
	redirection = malloc(sizeof(t_redirection));
	redirection->type = head->type;
	redirection->next_re = NULL;
	head = head->next;
	if (!head)
		return (free(redirection), NULL);
	redirection->file = head->value;
	head = head->next;
	if (head && head->type >= TOKEN_REDIR_IN && head->type <= TOKEN_HEREDOC)
		redirection->next_re = mk_redirection(head);
	return (redirection);
}

char	*expand_env_vars(const char *input)
{
	char	*result;
	char	*temp;
	char	*var_start;
	char	*var_end;
	char	*var_name;
	char	*var_value;
	size_t	new_len;

	if (!input)
		return (NULL);
	result = ft_strdup(input);
	if (!result)
		return (NULL);
	var_start = ft_strchr(result, '$');
	while (var_start)
	{
		var_end = var_start + 1;
		while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
			var_end++;
		if (var_end == var_start + 1)
		{
			var_start = ft_strchr(var_end, '$');
			continue;
		}
		var_name = ft_substr(var_start, 1, var_end - var_start - 1);
		if (!var_name)
			return (free(result), NULL);
		var_value = getenv(var_name);
		free(var_name);
		new_len = ft_strlen(result) - (var_end - var_start);
		if (var_value)
			new_len += ft_strlen(var_value);
		temp = malloc(new_len + 1);
		if (!temp)
			return (free(result), NULL);
		ft_strlcpy(temp, result, var_start - result + 1);
		if (var_value)
			ft_strlcat(temp, var_value, new_len + 1);
		ft_strlcat(temp, var_end, new_len + 1);
		free(result);
		result = temp;
		var_start = ft_strchr(result, '$');
	}
	return (result);
}

void	parc_args(t_ms *head, t_command *com)
{
	int	j;
	int	i;

	i = 0;
	j = commands(head);
	com->name = head->value;
	com->args = malloc(sizeof(char *) * (j + 1));

	while (i < j)
	{
		if (head->type == TOKEN_DQUOTE)
			com->args[i] = expand_env_vars(head->value);
		else
			com->args[i] = head->value;
		head = head->next;
		i++;
	}
	com->args[i] = NULL;
}

t_command	*mk_command(t_ms *head, t_command *prev)
{
	t_command	*command;

	if (!head)
		return (NULL);
	command = malloc(sizeof(t_command));
	parc_args(head, command);
	command->next = NULL;
	command->redirections = NULL;
	command->prev = prev;
	while (head->type >= TOKEN_WORD && head->type <= TOKEN_DQUOTE)
	{
		head = head->next;
		if (!head)
			break ;
	}
	if (head && head->type != TOKEN_PIPE)
	{
		command->redirections = mk_redirection(head);
		head = re_pipe(head);
	}
	if (head && head->type == TOKEN_PIPE)
		if (head->next)
			command->next = mk_command(head->next, command);
	return (command);
}
