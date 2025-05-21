/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:38:03 by sael-kha          #+#    #+#             */
/*   Updated: 2025/05/21 16:23:45 by sael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	print_token(t_ms *head)
// {
// 	while(head)
// 	{
// 		printf("%s type -> %d\n", head->value, head->type);
// 		head = head->next;
// 	}
// }

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

void	crtl_C(int sig)
{
	(void)sig;
	kill(SIGCHLD, SIGINT);
	waitpid(SIGCHLD, NULL, 0);
	printf("\n");
	rl_on_new_line();
	rl_replace_line("",0);
	rl_redisplay();
}

void	setup_sig()
{
	signal(SIGINT, crtl_C);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);
}

void	print_minishit(void)
{
	printf("+=========================================================================+\n");
	printf("|.___  ___.  __  .__   __.  __       _______. __    __   __  .___________.|\n");
	printf("||   \\/   | |  | |  \\ |  | |  |     /       ||  |  |  | |  | |           ||\n");
	printf("||  \\  /  | |  | |   \\|  | |  |    |   (----`|  |__|  | |  | `---|  |----`|\n");
	printf("||  |\\/|  | |  | |  . `  | |  |     \\   \\    |   __   | |  |     |  |     |\n");
	printf("||  |  |  | |  | |  |\\   | |  | .----)   |   |  |  |  | |  |     |  |     |\n");
	printf("||__|  |__| |__| |__| \\__| |__| |_______/    |__|  |__| |__|     |__|     |\n");
	printf("+=========================================================================+\n");
}

int	main(int ac,char **av,char **env)
{
	(void)av;
	t_command	*commands;
	char		*input;

	if (ac != 1)
		return (printf("no need for args\n"), 1);
	setup_sig();
	print_minishit();
	input = readline("minishell$ ");
	while (input)
	{
		add_history(input);
		commands = token_input(input);
		// print_com(commands);
		ft_executing(commands, env);
		input = readline("minishell$ ");
	}
}
