/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:28:47 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/05 23:09:21 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	make_fork(pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
		return (ft_printf(2, "%s\n", strerror(errno)), FALSE);
	return (TRUE);
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
	if (cmd->status != SYNTAX && cmd->err_msg)
		ft_printf(2, "%s", cmd->err_msg);
	close_fds(cmd);
	if (!ft_strnstr(cmd->err_msg, "Permission", 10))
		exit(126);
	else
		exit(127);
}
