/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:54:38 by sael-kha          #+#    #+#             */
/*   Updated: 2025/06/02 15:11:01 by sael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "./ft_libft/libft.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_SQUOTE,
	TOKEN_DQUOTE,
	TOKEN_PIPE,	
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV,
	TOKEN_EOF,
}	t_token_type;

typedef struct s_ms
{
	char		*value;
	int			type;
	struct s_ms	*next;
}	t_ms;

typedef struct s_redirection
{
	t_token_type			type;
	char					*file;
	struct s_redirection	*next_re;
}	t_redirection;

typedef struct s_command
{
	char				*name;
	char				**args;
	t_redirection		*redirections;
	int					pipe[2];
	struct s_command	*next;
	struct s_command	*prev;
}	t_command;

typedef struct s_env
{
	char	**vars;
	int		count;
	int		exit_s;
}			t_env;

void	print_token(t_ms *head);

void		GIRV(t_ms *head, t_env *env);
void		reset_signals(void);
int			commands(t_ms	*head);
void		ft_free(t_ms	*head);
int			check_sintax(t_ms *head);
t_command	*token_input(char *input, t_env *env);
char		*shorting_code(char *input, t_env *env);
void		link_list(t_ms *ms1, t_ms *ms2);
t_ms		*make_list(char *value, int type, ...);
t_command	*mk_command(t_ms *head, t_command *prev);
void		ft_executing(t_command	*commands, t_env *env);
t_ms		*process_token(int *s, int *e, char *str, t_ms **head);

/*			env				*/
t_env		*init_env(char **envp);
char		*ft_getenv(t_env *env, const char *key);


#endif