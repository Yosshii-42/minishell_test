
#include "../minishell.h"

static int	wait_process(void)//int count)
{
	int	status;
	int	exit_status;

	status = 0;
	while (1)
	{
		if (waitpid(-1, &status, 0) == -1)
		{
			if (errno == ECHILD)
				break;
			else
				return (ft_printf(2, "%s\n", strerror(errno)), EXIT_FAILURE);
		}
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		// if (WIFSIGNALED(status))
		// 	exit_status = WTERMSIG(status) + 128;
	}
	return (exit_status);
}

static void	parent_process(t_cmd *cmd)
{
	if (cmd->pp[0] > 0)
		dup2(cmd->pp[0], STDIN_FILENO);
	close_fds(cmd);
	free_cmd(cmd);
}

static void	child_process(t_cmd *cmd, char **path)
{
	printf("child");
	if (cmd->err_msg)
		exit_child_process(cmd);
	if (cmd->readfd > 0)
		dup2(cmd->readfd, 0);
	if (cmd->writefd > 0)
		dup2(cmd->writefd, 1);
	else if (cmd->pp[1] > 0)
		dup2(cmd->pp[1], STDOUT_FILENO);
	close_fds(cmd);
	close(3);
	if (execve(cmd->pathname, cmd->cmd, path) == -1)
	{
		ft_printf(2, "here\n");
		exit(1);
	}
}

int	run_process(char *line, char **path, char *pwd, int *original_stdin_fd)
{
	pid_t	pid;
	t_cmd	*cmd;
	// int		i;
	t_token	*token;
	t_token	*ptr;
	int		count;

	token = make_token_lst(line);
	count = cmd_count(token);
	ptr = token;
	// i = -1;
	while (count--)//++i < cmd_count(ptr))
	{
		cmd = NULL;
		// cmd = make_cmd(token, cmd, path, pwd);
		cmd = make_cmd(token, cmd, path, pwd);
		token = cmd->token;
		if (!token)
			break;
	printf("run_process token = %s, pathname = %s\n", token->word, cmd->pathname);
		if (!token)
			break;
		// while (!(token->status == END || token->kind == PIPE) && token->next)
		// 	token = token->next;
		// if (token->kind == PIPE && token->next)
		// 	token = token->next;
		if (!make_fork(&pid))
			return (token_lstclear(ptr), free_cmd(cmd), EXIT_FAILURE);
		if (pid == 0)
			child_process(cmd, path);
		else if (pid > 0)
			parent_process(cmd);
	}
	token_lstclear(ptr);
	dup2(*original_stdin_fd, STDIN_FILENO);
	close(*original_stdin_fd);
	return (wait_process());
}
