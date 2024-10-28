
#include "../minishell.h"

int	cmd_count(t_token *token)
{
	int		count;
	t_token	*ptr;

	count = 0;
	ptr = token;
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
void	make_fork(pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
		print_error_and_exit(strerror(errno));
}

void	close_fds(t_cmd *cmd)
{
	if (cmd->readfd > 0)
		close(cmd->readfd);
	if (cmd->writefd > 0)
		close(cmd->writefd);
	if (cmd->pp[0] > 0)
		close(cmd->pp[0]);
	if (cmd->pp[1] > 0)
		close(cmd->pp[1]);
}

void	exit_child_process(t_cmd *cmd)
{
	if (cmd->err_msg)
		ft_printf(2, "%s", cmd->err_msg);
	close_fds(cmd);
	exit(127);
}

void	token_lstclear(t_token *token)
{
	while (token)
	{
		if (token->kind == LIMITTER)
			unlink(FILE_NAME);
		if (token->word)
			free(token->word);
		if (token->next)
		{
			token = token->next;
			free(token->pre);
		}
		else
			break;
	}
}
