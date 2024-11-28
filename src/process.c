/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:50:40 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/27 22:01:03 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	wait_process(void)
{
	int	status;
	int	exit_status;

	status = 0;
	while (1)
	{
		if (waitpid(-1, &status, 0) == -1 && errno == ECHILD)
			break;
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
			if (exit_status == 256 - 2)
				exit(EXIT_FAILURE);
		}
		else if (WIFSIGNALED(status) && WTERMSIG(status) != SIGPIPE)
			exit_status = WTERMSIG(status) + 128;
	}
	if (exit_status > 255)
		return (EXIT_FAILURE);
	return (exit_status);
}

int	parent_process(t_cmd *cmd, t_token *token, int count)
{
	ignore_signal(SIGQUIT);
	ignore_signal(SIGINT);
	if (cmd->status == SYNTAX || (count == NO_PIPE && cmd->status == BUILTIN))
	{
		if (cmd->writefd > 0)
			safe_dup2(cmd->writefd, STDOUT_FILENO, PARENT, cmd);
		close_fds(cmd);
		if (cmd->status == SYNTAX)
			return (ft_printf(2, "bash: syntax error\n"), 2);
		if (cmd->err_msg)
			return (builtin_end_process(cmd, token));
		if (do_builtin(cmd) == false)
		{
			free_all(cmd);
			exit(end_status(GET, 0));
		}
		else
			return (end_status(GET, 0));
	}
	else if (cmd->pp[0] > 0)
		safe_dup2(cmd->pp[0], STDIN_FILENO, PARENT, cmd);
	close_fds(cmd);

	return (EXIT_SUCCESS);
}

static void	child_process(t_cmd *cmd, int stdio[2])
{
	child_signal();
	if (cmd->err_msg || !cmd->cmd)
		child_exit_process(cmd, stdio);
	if (cmd->readfd > 0)
		safe_dup2(cmd->readfd, STDIN_FILENO, CHILD, cmd);
	if (cmd->writefd > 0)
		safe_dup2(cmd->writefd, STDOUT_FILENO, CHILD, cmd);
	else if (cmd->pp[1] > 0)
		safe_dup2(cmd->pp[1], STDOUT_FILENO, CHILD, cmd);
	close_fds(cmd);
	close(stdio[0]);
	close(stdio[1]);
	if (check_builtin(cmd->cmd[0]) >= 0)
		exit((do_builtin(cmd), end_status(GET, 0)));
	if (!(cmd->cmd) || cmd->status == SYNTAX)
		exit(EXIT_SUCCESS);
	if (execve(cmd->pathname, cmd->cmd, NULL) == -1)
		execve_fail_process(cmd);
}

static bool	pipex_engine(t_cmd *cmd, t_token *token, int stdio[2])
{
	int	pid;

	make_fork(&pid);
	if (pid == 0)
		child_process(cmd, stdio);
	else if (pid > 0)
		parent_process(cmd, token, PIPE_EXIST);
	if (access(FILE_NAME, F_OK))
		unlink(FILE_NAME);
	return (true);
}

int	run_process(t_token *token, int *stdio, int command_count)
{
	t_token		*ptr;
	t_cmd		*cmd;
	int			command_flag;

	ptr = token;
	while (command_count--)
	{
		if (!token)
			break ;
		if (!expand_token(token))
			return (free_token(ptr), EXIT_FAILURE);
		cmd = NULL;
		command_flag = 0;
		cmd = make_cmd(token, cmd, command_flag);
		if (pipe_count(ptr) == 0
				&& (cmd->status == BUILTIN || cmd->status == SYNTAX))
			return (no_pipe_process(cmd, stdio));
		else if (pipex_engine(cmd, ptr, stdio) == false)
			return (syntax_end(cmd, stdio), end_status(GET, 0));
		if (cmd->status == SYNTAX)
			return (syntax_end(cmd, stdio), 2);
		token = cmd->token;
		free_cmd(cmd);
	}
	return (end_process(stdio), wait_process());
}
