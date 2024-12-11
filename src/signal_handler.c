/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:02:44 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/11 22:03:17 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_handler(int signum)
{
	(void) signum;
	g_sig_status = SIGINT;
	rl_replace_line("", 0);
	rl_done = 1;
	//close(0);
}

void	sig_handler(int signum)
{
	(void)signum;
	g_sig_status = SIGINT;
	rl_replace_line("", 0);
	rl_done = 1;
}
