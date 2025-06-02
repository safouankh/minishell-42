/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sintax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:48:20 by sael-kha          #+#    #+#             */
/*   Updated: 2025/06/01 18:45:43 by sael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	node_counter(t_ms *head)
{
	int	i;

	i = 0;
	while (head && (head->type >= TOKEN_WORD && head->type <= TOKEN_DQUOTE))
	{
		i++;
		head = head->next;
	}
	return (i);
}

t_ms	*ret_redorpipe(t_ms *head)
{
	while (head && (head->type >= TOKEN_WORD && head->type <= TOKEN_DQUOTE))
		head = head->next;
	return (head);
}

int	check_sintax(t_ms *head);

int	check_red(t_ms *head)
{
	int	i;

	if (!head || (head->type >= TOKEN_REDIR_IN && head->type <= TOKEN_HEREDOC))
		return (ft_putstr_fd("minishell: parse error near \\n\n", 2), 0);
	i = node_counter(head);
	if (i != 1)
	{
		if (i > 1)
			ft_putstr_fd("minishell: to many files for red\n", 2);
		else
			ft_putstr_fd("minishell: no files for red\n", 2);
		return (0);
	}
	head = ret_redorpipe(head);
	if (head && head->type == TOKEN_PIPE)
		return (check_sintax(head->next));
	if (head && (head->type >= TOKEN_REDIR_IN && head->type <= TOKEN_HEREDOC))
		return (check_red(head->next));
	return (1);
}

int	check_sintax(t_ms *head)
{
	if (!head || !(head->type >= TOKEN_WORD && head->type <= TOKEN_DQUOTE))
	{
		ft_putstr_fd("minishell: it have to be started with command\n", 2);
		return (0);
	}
	while (head && (head->type >= TOKEN_WORD && head->type <= TOKEN_DQUOTE))
		head = head->next;
	if (head && (head->type >= TOKEN_REDIR_IN && head->type <= TOKEN_HEREDOC))
		return (check_red(head->next));
	else if (head && head->type == TOKEN_PIPE)
		return (check_sintax(head->next));
	return (1);
}
