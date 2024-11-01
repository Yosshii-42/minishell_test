
#include "../minishell.h"

static int	wait_process(int count)
{
	int	status;
	int	i;
	int	exit_status;

	status = 0;
	i = -1;
	// 追加(SIGPIPE問題)
	exit_status = 0;
	while (++i < count)
	{
		if (waitpid(-1, &status, 0) == -1)
			print_error_and_exit(strerror(errno));
		//if (WIFEXITED(status))
		//	exit_status = WEXITSTATUS(status);
		exit_status = get_exit_status(status);
	}
	return (exit_status);
}

static void	parent_process(t_cmd *cmd, int i, int count)
{
	if (i != count - 1)
		dup2(cmd->pp[0], STDIN_FILENO);
	close_fds(cmd);
	ft_free_cmd(cmd);
}

static void	child_process(t_cmd *cmd, int i, int count)
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
	if (execve(cmd->pathname, cmd->cmd, NULL) == -1)
	{
		ft_printf(2, "here\n");
		exit(1);
	}
}

int	run_process(char *line, t_env *env)
{
	pid_t	pid;
	t_cmd	*cmd;
	int		i;
	t_token	*token;
	t_token	*ptr;

	token = make_token_lst(line);
	ptr = token;
	// while (ptr)
	// {
	// 	printf("token = %s, kind = %d\n", ptr->word, ptr->kind);
	// 	if (ptr->next)
	// 		ptr = ptr->next;
	// 	else
	// 		break;
	// }
	// printf("sizeof token = %lu\n", sizeof(t_token));
	// count = cmd_count(token);
	i = -1;
	while (++i < cmd_count(ptr))
	{
		cmd = NULL;
		cmd = make_cmd(token, cmd, env);
		while (!(token->end == END || token->kind == PIPE) && token->next)
			token = token->next;
		if (token->kind == PIPE && token->next)
			token = token->next;
		make_fork(&pid);
		if (pid == 0)
			child_process(cmd, i, cmd_count(ptr));
		else if (pid > 0)
			parent_process(cmd, i, cmd_count(ptr));
	}
	token_lstclear(ptr);
	// ft_free_split(cmd_line);
	return (wait_process(i));
}
