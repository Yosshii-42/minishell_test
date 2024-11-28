/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_safe_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:11:13 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/28 17:11:57 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*safe_malloc(size_t count, size_t size)
{
	void	*tmp;

	tmp = ft_calloc(size, count);
	if (!tmp)
		exit((ft_printf(2, "malloc: %s\n", strerror(errno)), EXIT_FAILURE));
	return (tmp);
}

int	safe_dup(int fd)
{
	int	new_fd;

	new_fd = -1;
	new_fd = dup(fd);
	if (new_fd == -1)
		exit((ft_printf(2, "dup: %s\n", strerror(errno)), EXIT_FAILURE));
	return (new_fd);
}

void	safe_dup2(int fd, int new_fd, int who, t_cmd *cmd)
{
	int	dup2_status;

	dup2_status = dup2(fd, new_fd);
	if (dup2_status == -1 && who == CHILD)
	{
		free_cmd(cmd);
		exit((ft_printf(2, "dup2: %s\n", strerror(errno)), EXIT_CHILD_DUP_ERR));
	}
	else if (dup2_status == -1 && who == PARENT)
	{
		free_all(cmd);
		exit((ft_printf(2, "dup2: %s\n", strerror(errno)), EXIT_FAILURE));
	}
}

void	safe_close(int *fd, int who, t_cmd *cmd)
{
	int	close_status;

	close_status = close(*fd);
	if (close_status == -1 && who == CHILD)
	{
		free_cmd(cmd);
		exit((ft_printf(2, "close: %s\n", strerror(errno)), EXIT_CHILD_DUP_ERR));
	}
	else if (close_status == -1 && who == PARENT)
	{
		free_all(cmd);
		exit((ft_printf(2, "close: %s\n", strerror(errno)), EXIT_FAILURE));
	}
}
