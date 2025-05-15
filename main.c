/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imatouil <imatouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:38:03 by sael-kha          #+#    #+#             */
/*   Updated: 2025/05/15 10:49:17 by imatouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token(t_ms *head)
{
	while(head)
	{
		printf("%s type -> %d\n", head->value, head->type);
		head = head->next;
	}
}
void	print_com(t_command *com);

void	print_red(t_redirection *red)
{
	printf("\tredirection\t\n");
	switch (red->type) {
		case TOKEN_REDIR_IN:    printf("< (input)\n"); break;
		case TOKEN_REDIR_OUT:   printf("> (output)\n"); break;
		case TOKEN_APPEND:      printf(">> (append)\n"); break;
		case TOKEN_HEREDOC:     printf("<< (heredoc)\n"); break;
		default:                printf("Unknown (%d)\n", red->type);
	}
	printf("%s\n",red->file);
	if (red->next_re)
		print_red(red->next_re);
}

void	print_com(t_command *com)
{
	if (!com)
		return ;
	printf("\tcommand\t\n");
	printf("command %s\n",com->name);
	for (int i = 0; com->args[i] != NULL; i++)
		printf("args[%d] %s\n", i, com->args[i]);
	if(com->redirections)
		print_red(com->redirections);
	if (com->next)
		print_com(com->next);
}

int	main(int ac, char **av, char **env)
{
	t_command	*commands;
	char	*input = readline("minishell$ ");
	while (input)
	{
		add_history(input);
		commands = token_input(input);
		// print_com(commands);
		executing(commands, env);
		input = readline("minishell$ ");
	}
}