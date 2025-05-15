#include "minishell.h"

int	commands(t_ms *head)
{
	int i = 0;
	while (head->type == TOKEN_WORD || head->type == TOKEN_SQUOTE || head->type == TOKEN_DQUOTE)
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
	{
		head = head->next;
	}
	return head;
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
	return redirection;
}

t_command	*mk_command(t_ms *head)
{
	t_command	*command;

	if (!head)
		return (NULL);
	int j = commands(head);
	command = malloc(sizeof(t_command));
	command->name = head->value;
	command->args = malloc(sizeof(char *) * (j + 1));
	int i = 0;
	while (i < j)
	{
		command->args[i] = head->value;
		head = head->next;
		i++;
	}
	command->args[i] = NULL;
	command->next = NULL;
	command->redirections = NULL;
	if (head && head->type != TOKEN_PIPE)
	{
		command->redirections = mk_redirection(head);
		head = re_pipe(head);
	}
	if (head && head->type == TOKEN_PIPE)
		if (head->next)
			command->next = mk_command(head->next);
	return (command);
}
