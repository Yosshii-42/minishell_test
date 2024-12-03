/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:50:40 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/03 17:05:21 by hurabe           ###   ########.fr       */
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
			exit_status = WTERMSIG(status) + 128;
	}
	if (exit_status > 255)
		return (EXIT_FAILURE);
	return (exit_status);
}

int	parent_process(t_cmd *cmd, int count)
{
	ignore_signal(SIGQUIT);
	ignore_signal(SIGINT);
	if (cmd->status == SYNTAX)
		return (ft_printf(2, "bash: syntax error\n"), 2);
	else if (cmd->err_msg)
		return (builtin_end_process(cmd));
	else if (cmd->writefd > 0)
	{
		dup2(cmd->writefd, STDOUT_FILENO);
		close_fds(cmd);
	}
	else if (!cmd->pathname && cmd->status != BUILTIN)
		return (EXIT_SUCCESS);
	else if (count == NO_PIPE && cmd->status == BUILTIN && do_builtin(cmd) == false)
		exit((free_all(cmd), end_status(GET, 0)));
	else 
	{
		if (cmd->pp[0] > 0)
			dup2(cmd->pp[0], STDIN_FILENO);
		return (close_fds(cmd), EXIT_SUCCESS);
	}
	return (end_status(GET, 0));
}

char	**make_env_array(void)
{
	char	**env_lst;
	t_env	*env;
	int		count;

	env = set_env(GET, NULL);
	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	env_lst = (char **)safe_malloc(count + 1, sizeof(char *));
	env = set_env(GET, NULL);
	count = 0;
	while (env)
	{
		env_lst[count] = strjoin_with_free(env->key, "=", NO_FREE);
		env_lst[count] = strjoin_with_free(env_lst[count], env->value, FREE_S1);
		env = env->next;
		count++;
	}
	env_lst[count] = NULL;
	return (env_lst);
}

static void	child_process(t_cmd *cmd, int stdio[2])
{
	char	**envp;

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
	envp = make_env_array();
	if (execve(cmd->pathname, cmd->cmd, envp) == -1)
		execve_fail_process(cmd);
}


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

int	run_process(t_token *token, int *stdio, int command_count)
{
	t_token		*ptr;
	t_cmd		*cmd;
	int			command_flag;

	ptr = token;
	//printf("aaatoken = %s\n", token->word);
	while (command_count--)
	{
		if (!token)
			break ;
		//expand_token(token);
		cmd = NULL;
		command_flag = 0;
		cmd = make_cmd(token, cmd, command_flag);
		if (pipe_count(ptr) == 0 \
				&& (cmd->status == BUILTIN || cmd->status == SYNTAX || !(cmd->pathname)))
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
