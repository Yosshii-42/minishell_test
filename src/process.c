/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:28:54 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/09 01:23:05 by tsururukako      ###   ########.fr       */
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

void	end_process(t_token *token, int *original_stdin)
{
	// printf("here end process token = %s\n", token->word);
	free_token(token);
	dup2(*original_stdin, STDIN_FILENO);
	close(*original_stdin);
}

static void	parent_process(t_cmd *cmd)
{
	if (cmd->status == SYNTAX)
		ft_printf(2, "bash: syntax error\n");
	else if (cmd->pp[0] > 0)
		dup2(cmd->pp[0], STDIN_FILENO);
	close_fds(cmd);
}

static void	child_process(t_cmd *cmd, char **path, int *original_stdin)
{
	if (cmd->err_msg)
		exit_child_process(cmd);
	if (cmd->readfd > 0)
		dup2(cmd->readfd, 0);
	if (cmd->writefd > 0)
		dup2(cmd->writefd, 1);
	else if (cmd->pp[1] > 0)
		dup2(cmd->pp[1], STDOUT_FILENO);
	close_fds(cmd);
	close(*original_stdin);
	if (!(cmd->cmd) || cmd->status == SYNTAX)
		exit(EXIT_SUCCESS);
	if (!ft_memcmp(cmd->cmd[0], "exit", ))
		builtin_exit(cmd);
	if (execve(cmd->pathname, cmd->cmd, path) == -1)
	{
		ft_printf(2, "here\n");
		exit(1);
	}
}

void	syntax_end(t_cmd *cmd, t_token *token, int *stdin)
{
	if (cmd)
		free_cmd(cmd);
	free_token(token);
	dup2(*stdin, STDIN_FILENO);
	close(*stdin);
}

int	run_process(t_token *token, char **path, char *pwd, int *original_stdin)
{
	pid_t	pid;
	t_cmd	*cmd;
	t_token	*ptr;
	int		count;

	count = cmd_count(token);
	ptr = token;
	while (count--)
	{
		cmd = NULL;
		if ((cmd = make_cmd(token, cmd, path, pwd)) && !cmd)
			return (end_process(ptr, original_stdin), -1);
		if ((token = cmd->token) && !token)
			break ;
		if (!make_fork(&pid))
			return (free_token(ptr), free_cmd(cmd), EXIT_FAILURE);
		if (pid == 0)
			child_process(cmd, path, original_stdin);
		else if (pid > 0)
			parent_process(cmd);
		if (cmd->status == SYNTAX)
			return (syntax_end(cmd, ptr, original_stdin), 2);
		token = cmd->token;
		free_cmd(cmd);
	}
	end_process(ptr, original_stdin);
	return (wait_process());
}
