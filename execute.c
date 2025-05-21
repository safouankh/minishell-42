#include "minishell.h"

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

void	ft_exec(t_command	*commands,char **env)
{
	char	*com_path;
	char	*path;
	char	**paths;
	char	*temp;
	int		i;

	path = getenv("PATH");
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
			execve(com_path,commands->args,env);
		}
		free(com_path);
		i++;
	}
	if (commands->name[0] == '/' || commands->name[0] == '.')
		execve(commands->name,commands->args,env);
	printf("error command %s not found\n", commands->name);
}

void	ft_executing(t_command	*commands,char **env)
{
	int			pid;
	int			status;

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
		wait(&status);
}
