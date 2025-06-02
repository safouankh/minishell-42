#include "minishell.h"

// typedef struct s_herdoc
// {
// 	char	*del;
// 	int		pipe[2];
// }	t_herdoc;

// int	setup_all_herdoc(t_redirection *redirs)
// {
// 	t_herdoc	herdocs[16];
	
// }

int setup_redirections(t_redirection *redirs)
{
	int	fd;

	while (redirs)
	{
		if (redirs->type == TOKEN_REDIR_IN)
			fd = open(redirs->file, O_RDONLY);
		else if (redirs->type == TOKEN_REDIR_OUT)
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redirs->type == TOKEN_APPEND)
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		// else if (redirs->type == TOKEN_HEREDOC)
		// 	fd = handle_heredoc(redirs->file);
		if (fd == -1)
			return (perror("minishell"), 0);
		if (redirs->type == TOKEN_REDIR_IN || redirs->type == TOKEN_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redirs = redirs->next_re;
	}
	return (1);
}

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while(paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

void	ft_exec(t_command	*commands,t_env *env)
{
	char	*com_path;
	char	*path;
	char	**paths;
	char	*temp;
	int		i;

	if (commands->name[0] == '/' || commands->name[0] == '.')
		if(execve(commands->name,commands->args,env->vars) == -1)
			return (perror("minishell"));
	path = ft_getenv(env, "PATH");
	paths = ft_split(path,':');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		com_path = ft_strjoin(temp, commands->name);
		free(temp);
		if(access(com_path, X_OK) == 0)
		{
			reset_signals();
			free_paths(paths);
			execve(com_path,commands->args,env->vars);
			free(com_path);
            ft_putstr_fd("error in execve\n", 2);
            return;
		}
		free(com_path);
		i++;
	}
	free_paths(paths);
	printf("error command %s not found\n", commands->name);
}

void	ft_executing(t_command	*commands,t_env *env)
{
	int			pid;

	if (!commands)
		return ;
	pid = fork();
	if (pid == 0)
	{
		if (commands->redirections)
		{
			if (!setup_redirections(commands->redirections))
				exit(EXIT_FAILURE);
			ft_exec(commands, env);
			exit(EXIT_FAILURE);
		}
		else
		{
			ft_exec(commands,env);
			exit(EXIT_FAILURE);
		}
	}else
		wait(&env->exit_s);
}
