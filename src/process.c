
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
			print_error_and_exit(strerror(errno));
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	return (exit_status);
}

static void	parent_process(t_cmd *cmd, int i, int count)
{
	if (i != count - 1)
		dup2(cmd->pp[0], STDIN_FILENO);
	close_fds(cmd);
	free_cmd(cmd);
}

static void	child_process(t_cmd *cmd, int i, int count, char **path)
{
	if (cmd->err_msg)
		exit_child_process(cmd);
	if (cmd->readfd > 0)
		dup2(cmd->readfd, 0);
	if (cmd->writefd > 0)
		dup2(cmd->writefd, 1);
	else if (i != count - 1 && dup2(cmd->pp[1], STDOUT_FILENO) == -1)
		print_error_and_exit(strerror(errno));
	close_fds(cmd);
	if (execve(cmd->pathname, cmd->cmd, path) == -1)
	{
		ft_printf(2, "here\n");
		exit(1);
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
		while (!(token->end == END || token->kind == PIPE) && token->next)
			token = token->next;
		if (token->kind == PIPE && token->next)
			token = token->next;
		make_fork(&pid);
		if (pid == 0)
			child_process(cmd, i, cmd_count(ptr), path);
		else if (pid > 0)
			parent_process(cmd, i, cmd_count(ptr));
	}
	token_lstclear(ptr);
	return (wait_process(i));
}
