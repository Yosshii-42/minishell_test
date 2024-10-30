
#include "../minishell.h"

static int	wait_process(int count)
{
	int	status;
	int	i;
	int	exit_status;

	status = 0;
	i = -1;
	while (++i < count)
	{
		if (waitpid(-1, &status, 0) == -1)
			return (ft_printf(2, "%s\n", strerror(errno)), EXIT_FAILURE);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
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
	if (cmd->err_msg)
		exit_child_process(cmd);
	if (cmd->readfd > 0)
		dup2(cmd->readfd, STDIN_FILENO);
	if (cmd->writefd > 0)
		dup2(cmd->writefd, STDOUT_FILENO);
	else if (cmd->pp[1] > 0)
		dup2(cmd->pp[1], STDOUT_FILENO);
	close_fds(cmd);
	if (execve(cmd->pathname, cmd->cmd, path) == -1)
	{
		ft_printf(2, "here\n");
		exit(127);
	}
}

int	run_process(char *line, char **path, char *pwd)
{
	pid_t	pid;
	t_cmd	*cmd;
	int		i;
	t_token	*token;
	t_token	*ptr;

	token = make_token_lst(line);
	ptr = token;
	i = -1;
	while (++i < cmd_count(ptr))
	{
		cmd = NULL;
		cmd = make_cmd(token, cmd, path, pwd);
		while (!(token->status == END || token->kind == PIPE) && token->next)
			token = token->next;
		if (token->kind == PIPE && token->next)
			token = token->next;
		if (!make_fork(&pid))
			return (token_lstclear(ptr), free_cmd(cmd), EXIT_FAILURE);
		if (pid == 0)
			child_process(cmd, path);
		else if (pid > 0)
			parent_process(cmd);
	}
	token_lstclear(ptr);
	return (wait_process(i));
}
