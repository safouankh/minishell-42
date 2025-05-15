#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "./LIBFT/libft.h"

typedef enum e_token_type {
	TOKEN_WORD,			// Regular command/argument
	TOKEN_PIPE,			// |
	TOKEN_REDIR_IN,		// <
	TOKEN_REDIR_OUT,	// >
	TOKEN_APPEND,		// >>
	TOKEN_HEREDOC,		// <<
	TOKEN_SQUOTE,		// '
	TOKEN_DQUOTE,		// "
	// TOKEN_EOF			// End of input
}	t_token_type;

typedef struct s_ms
{
	char		*value;
	int			type;
	struct s_ms *next;
}				t_ms;

typedef struct s_redirection
{
    t_token_type			type; 	//type dyal redirection
    char					*file;	// file
    struct s_redirection	*next_re;	//next redirection ida kanet
} t_redirection;

typedef struct s_command
{
    char				*name; // command name
    char				**args; // array of args
    t_redirection		*redirections; // pointer to redirection ida kan
    int					pipe[2]; // array of file descriptor
    struct s_command	*next; //next comand ida kan
} t_command;

t_command	*token_input(char *input);
int			commands(t_ms	*head);
t_command	*mk_command(t_ms *head);
void		executing(t_command	*commands,char **env);

#endif