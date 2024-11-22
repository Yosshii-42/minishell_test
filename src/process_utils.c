/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:28:47 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/20 15:27:25 by yotsurud         ###   ########.fr       */
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

int	pipe_count(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		if (*(token->word) == '|' )
		{
			count++;
			break ;
		}
		token = token->next;
	}
	return (count);
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
