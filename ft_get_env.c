/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 13:53:48 by sael-kha          #+#    #+#             */
/*   Updated: 2025/06/02 16:27:18 by sael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ms	*mk_list(char *str)
{
	t_ms	*head;
	int		start;
	int		end;

	head = NULL;
	start = 0;
	end = 0;
	if (!str)
		return (NULL);
	while (str[start])
		process_token(&start, &end, str, &head);
	return (head);
}

void	give_value(t_ms *head, t_env *env)
{
	char	*val;

	while (head)
	{
		if (head->type == TOKEN_ENV)
		{
			val = ft_getenv(env, head->value);
			if (head->value[0] == '?')
			{
				free(head->value);
				head->value = ft_itoa(env->exit_s);
			}
			else if (!val)
			{
				free(head->value);
				head->value = ft_strdup("");
			}
			else
			{
				free(head->value);
				head->value = ft_strdup(val);
			}
		}
		head = head->next;
	}
}

char	*join_str(t_ms *head)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	while (head)
	{
		tmp = result;
		result = ft_strjoin(result, head->value);
		free(tmp);
		head = head->next;
	}
	return (result);
}

void	ft_free(t_ms	*head)
{
	t_ms	*curent;

	while (head)
	{
		curent = head;
		head = head->next;
		free(curent->value);
		free(curent);
	}
}

char	*shorting_code(char *input, t_env *env)
{
	t_ms	*head;
	char	*str;

	head = mk_list(input);
	give_value(head, env);
	str = join_str(head);
	ft_free(head);
	return (str);
}
