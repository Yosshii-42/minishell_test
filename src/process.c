
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
	// if (info->here_doc == YES)
	// {
	// 	unlink(FILE_NAME);
	// 	info->here_doc = NO;
	// }
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
	// if (cmd->readfd > 0)
	// 	dup2(cmd->readfd, 0);
	// if (cmd->writefd > 0)
	// 	dup2(cmd->writefd, 1);
	// else
	if (i != count - 1 && dup2(cmd->pp[1], STDOUT_FILENO) == -1)
		print_error_and_exit(strerror(errno));
	close_fds(cmd);
	if (execve(cmd->pathname, cmd->cmd, NULL) == -1)
	{
		ft_printf(2, "here\n");
		exit(1);
	}
}

static void	make_fork(pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
		print_error_and_exit(strerror(errno));
}

// char	**split_by_pipe(char *line)
// {
// 	char	**split;

// 	split = ft_split(line, '|');
// 	if (!split)
// 		return (NULL);
// 	return (split);
// }

static void	lstclear(t_token *token)
{
	while (token)
	{
		free(token->word);
		free(token);
		if (token->next)
			token = token->next;
		else
			break;
	}
}


// int	cmd_count(char **cmd_line)
int	cmd_count(t_token *token)
{
	int		count;
	t_token	*ptr;

	count = 0;
	ptr = token;
	// while (cmd_line[count])
	if (!token)
		return (0);
	while (ptr)
	{
		if (*(ptr->word) == '|')
			count++;
		ptr = ptr->next;
	}
	return (count + 1);
}

int	run_process(char *line, t_env *env)
{
	pid_t	pid;
	t_cmd	*cmd;
	int		i;
	int		count;
	// char	**cmd_line;
	t_token	*token;
	t_token	*ptr;

	// cmd_line = NULL;
	token = split_by_space(line);
	ptr = token;
	while (ptr)
	{
		printf("token = %s\tkind = %d\n", ptr->word, ptr->kind);
		ptr = ptr->next;
	}
	// printf("sizeof token = %lu\n", sizeof(t_token));
	// return (0);
	// cmd_line = split_by_pipe(line);
	// count = cmd_count(cmd_line);
	count = cmd_count(token);
printf("here\n");
	i = -1;
	// while (cmd_line[++i])
	while (++i <= count)
	{
		cmd = NULL;
		// cmd = make_cmd(cmd_line[i], env);
		cmd = make_cmd(token, env);
		make_fork(&pid);
		if (pid == 0)
			child_process(cmd, i, count);
		else if (pid > 0)
			parent_process(cmd, i, count);
	}
	lstclear(ptr);
	// ft_free_split(cmd_line);
	return (wait_process(i));
}
