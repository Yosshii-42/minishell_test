/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:50:40 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/27 01:17:26 by tsururukako      ###   ########.fr       */
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
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status) && WTERMSIG(status) != SIGPIPE)
			exit_status = WTERMSIG(status) + 128;
	}
	if (exit_status > 255)
		return (EXIT_FAILURE);
	return (exit_status);
}

// void	end_process(int stdio[2])
// {
// 	free_token(set_token(GET, NULL));
// 	if (dup2(stdio[0], STDIN_FILENO) == -1)
// 		exit((ft_printf(2, "dup2: %s\n", strerror(errno)), EXIT_FAILURE));
// 	if (dup2(stdio[1], STDOUT_FILENO) == -1)
// 		exit((ft_printf(2, "dup2: %s\n", strerror(errno)), EXIT_FAILURE));
// 	close(stdio[0]);
// 	close(stdio[1]);
// }

// void	parent_process(t_cmd *cmd)
// {
// 	if (cmd->status == SYNTAX)
// 		ft_printf(2, "bash: syntax error\n");
// 	else if (cmd->pp[0] > 0)
// 		dup2(cmd->pp[0], STDIN_FILENO);
// 	close_fds(cmd);

// 	return (EXIT_SUCCESS);
// }
// int	parent_process(t_cmd *cmd, t_token *token, int count)
int	parent_process(t_cmd *cmd, int count)
{
	ignore_signal(SIGQUIT);
	ignore_signal(SIGINT);
	if (cmd->status == SYNTAX || (count == NO_PIPE && cmd->status == BUILTIN) || !cmd->cmd[0])
	{
		if (cmd->writefd > 0)
			safe_dup2(cmd->writefd, STDOUT_FILENO, PARENT, cmd);
		close_fds(cmd);
		if (cmd->status == SYNTAX)
			return (ft_printf(2, "bash: syntax error\n"), 2);
		if (cmd->err_msg)
			return (builtin_end_process(cmd));
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
	if (!(cmd->cmd) || cmd->status == SYNTAX)
		exit(EXIT_SUCCESS);
	if (execve(cmd->pathname, cmd->cmd, cmd->path) == -1)
		execve_fail_process(cmd);
}

// void	syntax_end(t_cmd *cmd, int *stdin)
// {
// 	// if (cmd)
// 	// 	free_cmd(cmd);
// 	// free_token(token);
// 	// dup2(*stdin, STDIN_FILENO);
// 	// close(*stdin);
// 	if (cmd)
// 		free_cmd(cmd);
// 	end_process(stdio);
// }

// static bool	pipex_engine(t_cmd *cmd, t_token *token, int stdio[2])
static bool	pipex_engine(t_cmd *cmd, int stdio[2])
{
	int	pid;

	make_fork(&pid);
	if (pid == 0)
		child_process(cmd, stdio);
	else if (pid > 0)
		parent_process(cmd, PIPE_EXIST);
	if (access(FILE_NAME, F_OK))
		unlink(FILE_NAME);
	return (true);
}

// int	run_process(t_token *token, char **path, char *pwd, int *original_stdin)
// int	run_process(t_token *token, int *stdio, int command_count)
// {
// 	// pid_t	pid;
// 	t_cmd	*cmd;
// 	t_token	*ptr;
// 	int		command_flag;
// 	// int		count;

// 	// count = cmd_count(token);
// 	ptr = token;
// 	while (command_count--)
// 	{
// 		if (!token)
// 			break ;
// 		cmd = NULL;
// 		command_flag = 0;
// 		cmd = make_cmd(token, cmd, command_flag);
// 		if (pipe_count(ptr) == 0
// 				&& (cmd->status == BUILTIN || cmd->status == SYNTAX || !cmd->cmd[0]))
// 			return (no_pipe_process(cmd, stdio));
// 		// else if (pipex_engine(cmd, ptr, stdio) == false)
// 		else if (pipex_engine(cmd, stdio) == false)
// 			return (syntax_end(cmd, stdio), end_status(GET, 0));
// 		// if (cmd->status == SYNTAX)
// 		// 	return (syntax_end(cmd, ptr, original_stdin), 2);
// 		token = cmd->token;
// 		free_cmd(cmd);
// 	}
// 	end_process(ptr, original_stdin);
// 	return (wait_process());
// }
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
		else if (pipex_engine(cmd, stdio) == false)
			return (syntax_end(cmd, stdio), end_status(GET, 0));
		if (cmd->status == SYNTAX)
			return (syntax_end(cmd, stdio), 2);
		token = cmd->token;
		free_cmd(cmd);
	}
	return (end_process(stdio), wait_process());
}
