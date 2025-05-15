#include "minishell.h"

int setup_redirections(t_redirection *redirs)
{
	while (redirs)
	{
		int fd;
		if (redirs->type == TOKEN_REDIR_IN)
			fd = open(redirs->file, O_RDONLY);
		else if (redirs->type == TOKEN_REDIR_OUT)
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redirs->type == TOKEN_APPEND)
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		// else if (redirs->type == TOKEN_HEREDOC)
		//     fd = handle_heredoc(redirs->file); // You'll need to implement this
		if (fd == -1)
			return (perror("minishell"), 0);
		if (redirs->type == TOKEN_REDIR_IN || redirs->type == TOKEN_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redirs = redirs->next_re;
	}
    return 1;
}

void	executing(t_command	*commands,char **env)
{
	int	pid = fork();
	if (pid == 0)
	{
		if (commands->redirections)
		{
			if (setup_redirections(commands->redirections))
				return ;
		}
		char	*com_path;
		char	*path = getenv("PATH");
		char	**paths = ft_split(path,':');
		int		i = 0;
		while (paths[i])
		{
			char *temp = ft_strjoin(paths[i], "/");
			com_path = ft_strjoin(temp, commands->name);
			free(temp);
			if(access(com_path, X_OK) == 0)
				execve(com_path,commands->args,env);
			i++;
		}
		if (commands->name[0] == '/' || commands->name[0] == '.')
			execve(commands->name,commands->args,env);
		printf("error command %s not found\n", commands->name);
	}
	else
		wait(NULL);
}
