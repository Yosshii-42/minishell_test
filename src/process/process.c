/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 09:22:15 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/12 16:32:21 by yotsurud         ###   ########.fr       */
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
			break ;
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status) && WTERMSIG(status) != SIGPIPE)
		{
			exit_status = WTERMSIG(status) + 128;
			if (exit_status != 130)
				core_dump_signal(exit_status);
			else
				ft_putstr_fd("\n", STDOUT_FILENO);
		}
	}
	if (exit_status > 255)
		return (EXIT_FAILURE);
	else
		rl_on_new_line();
	return (exit_status);
}

static void	child_process(t_cmd *cmd, int stdio[2])
{
	char	**envp;

	exec_child_signal();
	if (cmd->err_msg || (!cmd->pathname && cmd->status != BUILTIN))
		child_exit_process(cmd, stdio);
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
		exit((do_builtin(cmd), end_status(GET, 0)));
	envp = make_env_array();
	if (execve(cmd->pathname, cmd->cmd, envp) == -1)
		execve_fail_process(cmd);
}

int	parent_process(t_cmd *cmd, int count)
{
	if (cmd->status == SYNTAX)
		return (ft_printf(2, "bash: syntax error\n"), 2);
	if (count == NO_PIPE && cmd->writefd > 0)
	{
		dup2(cmd->writefd, STDOUT_FILENO);
		close_fds(cmd);
	}
	else if (count == NO_PIPE && cmd->err_msg)
		return (ft_printf(2, "%s", cmd->err_msg), close_fds(cmd), EXIT_SUCCESS);
	if (count == NO_PIPE && cmd->status == BUILTIN)
	{
		if (do_builtin(cmd) == false)
			exit((free_all(cmd), end_status(GET, 0)));
		else
			return (end_status(GET, 0));
	}
	else
	{
		if (cmd->pp[0] > 0)
			dup2(cmd->pp[0], STDIN_FILENO);
		return (close_fds(cmd), EXIT_SUCCESS);
	}
	return (end_status(GET, 0));
}

static void	pipex_engine(t_cmd *cmd, int stdio[2])
{
	int	pid;

	make_fork(&pid);
	if (pid == 0)
		child_process(cmd, stdio);
	else if (pid > 0)
		parent_process(cmd, PIPE_EXIST);
	if (access(FILE_NAME, F_OK))
		unlink(FILE_NAME);
}

int	run_process(t_token *token, int *stdio, int command_count)
{
	t_cmd		*cmd;
	int			command_flag;
	int			pipe_flag;
	t_token		*ptr;

	ptr = set_token(GET, NULL);
	pipe_flag = command_count;
	while (command_count--)
	{
		if (!token)
			break ;
		cmd = NULL;
		cmd = (t_cmd *)safe_malloc(1, sizeof(t_cmd));
		init_cmd(cmd, pipe_flag);
		command_flag = 0;
		cmd = make_cmd(token, cmd, command_flag);
		if (cmd->status == SYNTAX || (!cmd->pathname && cmd->status != BUILTIN)
			|| (pipe_count(ptr) == 0 && cmd->status == BUILTIN)
			|| cmd->heredoc_sigint == true)
			return (no_fork_process(cmd, stdio));
		pipex_engine(cmd, stdio);
		token = cmd->token;
		free_cmd(cmd);
	}
	return (end_process(stdio), free_token(ptr), wait_process());
}
