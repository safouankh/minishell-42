/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parc_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 18:23:20 by sael-kha          #+#    #+#             */
/*   Updated: 2025/06/02 11:08:24 by sael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	commands(t_ms *head)
{
	int	i;

	i = 0;
	while (head->type == TOKEN_WORD 
		|| head->type == TOKEN_SQUOTE || head->type == TOKEN_DQUOTE)
	{
		i++;
		head = head->next;
		if (!head || head->type == TOKEN_EOF)
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
		return (NULL);
	redirection = malloc(sizeof(t_redirection));
	redirection->type = head->type;
	redirection->next_re = NULL;
	head = head->next;
	if (!head || head->type == TOKEN_EOF)
		return (free(redirection), NULL);
	redirection->file = ft_strdup(head->value);
	head = head->next;
	if (head && head->type >= TOKEN_REDIR_IN && head->type <= TOKEN_HEREDOC)
		redirection->next_re = mk_redirection(head);
	return (redirection);
}

void	parc_args(t_ms *head, t_command *com)
{
	int	j;
	int	i;

	i = 0;
	j = commands(head);
	com->name = ft_strdup(head->value);
	com->args = malloc(sizeof(char *) * (j + 1));
	while (i < j)
	{
		com->args[i] = ft_strdup(head->value);
		head = head->next;
		i++;
	}
	com->args[i] = NULL;
}

t_command	*mk_command(t_ms *head, t_command *prev)
{
	t_command	*command;

	if (!head || head->type == TOKEN_EOF)
		return (NULL);
	command = malloc(sizeof(t_command));
	parc_args(head, command);
	command->next = NULL;
	command->redirections = NULL;
	command->prev = prev;
	while (head->type >= TOKEN_WORD && head->type <= TOKEN_DQUOTE)
	{
		head = head->next;
		if (!head || head->type == TOKEN_EOF)
			break ;
	}
	if (head && head->type != TOKEN_PIPE)
	{
		command->redirections = mk_redirection(head);
		head = re_pipe(head);
	}
	if (head && head->type == TOKEN_PIPE)
		if (head->next || head->type != TOKEN_EOF)
			command->next = mk_command(head->next, command);
	return (command);
}
