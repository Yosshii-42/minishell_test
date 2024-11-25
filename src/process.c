/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-25 10:50:40 by yotsurud          #+#    #+#             */
/*   Updated: 2024-11-25 10:50:40 by yotsurud         ###   ########.fr       */
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
		if (waitpid(-1, &status, 0) == -1)
		{
			if (errno == ECHILD)
				break ;
			else
				return (ft_printf(2, "%s\n", strerror(errno)), EXIT_FAILURE);
		}
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status) && WTERMSIG(status) != SIGPIPE)
			exit_status = WTERMSIG(status) + 128;
	}
	if (exit_status > 255)
		return (EXIT_FAILURE);
	return (exit_status);
}

static int	parent_process(t_cmd *cmd, t_token *token, int count)
{
	if (count == NO_PIPE && cmd->status == BUILTIN)
	{
		if (cmd->writefd > 0)
			dup2(cmd->writefd, STDOUT_FILENO);
		close_fds(cmd);
		if (cmd->status == SYNTAX)
			return (ft_printf(2, "bash: syntax error\n"), 2);
		if (cmd->err_msg)
			return (builtin_end_process(cmd, token));
		if (do_builtin(cmd) == false)
			exit(end_status(GET, 0));
		else
			return (end_status(GET, 0));
	}
	if (cmd->status == SYNTAX)
		ft_printf(2, "bash: syntax error\n");
	else if (cmd->pp[0] > 0)
		dup2(cmd->pp[0], STDIN_FILENO);
	close_fds(cmd);
	return (EXIT_SUCCESS);
}

static void	child_process(t_cmd *cmd, t_token *token, int stdio[2])
{
	if (cmd->err_msg || !cmd->cmd)
		child_exit_process(cmd, token, stdio);
	if (cmd->readfd > 0)
		dup2(cmd->readfd, STDIN_FILENO);
	if (cmd->writefd > 0)
		dup2(cmd->writefd, STDOUT_FILENO);
	else if (cmd->pp[1] > 0)
		dup2(cmd->pp[1], STDOUT_FILENO);
	close_fds(cmd);
	close(stdio[0]);
	close(stdio[1]);
	if (check_builtin(cmd->cmd[0]) >= 0)
	{
		do_builtin(cmd);
		exit(end_status(GET, 0));
	}
	if (!(cmd->cmd) || cmd->status == SYNTAX)
		exit(EXIT_SUCCESS);
	if (execve(cmd->pathname, cmd->cmd, cmd->path) == -1)
	{
		exit(EXIT_FAILURE);
	}
}

static bool	minishell_engine(t_cmd *cmd, t_token *token, int stdio[2])
{
	int	pid;

	if (!make_fork(&pid))
		return (ft_printf(2, "fork error: %s", strerror(errno)), false);
	if (pid == 0)
		child_process(cmd, token, stdio);
	else if (pid > 0)
		parent_process(cmd, token, PIPE_EXIST);
	return (true);
}

int	run_process(t_token *token, int *stdio)
{
	t_cmd		*cmd;
	t_token		*ptr;
	int			count;

	ptr = token;
	count = cmd_count(token);
	while (count--)
	{
		if (!token)
			break ;
		if (!expand_token(token))
			return (free_token(ptr), EXIT_FAILURE);
		cmd = NULL;
		cmd = make_cmd(token, cmd);
		if (!cmd)
			return (end_process(ptr, stdio), 1);
		if (pipe_count(ptr) == 0 && cmd->status == BUILTIN)
		{
			cmd->count = 1;
			end_status(SET, parent_process(cmd, ptr, NO_PIPE));
			return (syntax_end(cmd, ptr, stdio), end_status(GET, 0));
		}
		else if (minishell_engine(cmd, ptr, stdio) == false)
			return (syntax_end(cmd, ptr, stdio), end_status(GET, 0));
		if (cmd->status == SYNTAX)
			return (syntax_end(cmd, ptr, stdio), 2);
		token = cmd->token;
		free_cmd(cmd);
	}
	end_process(ptr, stdio);
	return (wait_process());
}
