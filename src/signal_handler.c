/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:29:17 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/29 18:11:25 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	event(void)
{
	return (0);
}

void	perror_prestr(void)
{
	write(STDERR_FILENO, ERROR_PRESTR, ft_strlen(ERROR_PRESTR));
}

void	fatal_error_exit(char *err_msg)
{
	perror_prestr();
	perror(err_msg);
	exit(EXIT_FAILURE);
}

void	signal_handler(int signum)
{
	(void)signum;
	ft_printf(2, "DEBUG: g_sig_status=%d\n", g_sig_status);
	if (g_sig_status == HEREDOC)
	{
		g_sig_status = SIG_INT;
		rl_replace_line("", 0);
		rl_done = 1;
	}
	else if (g_sig_status == IN_CMD)
	{
		g_sig_status = SIG_INT;
		rl_on_new_line();
		rl_replace_line("\n", 0);
		rl_redisplay();
	}
	else
	{
		g_sig_status = SIG_INT;
		rl_replace_line("", 0);
		rl_on_new_line();
		write(STDERR_FILENO, "^C\n", 3);
		rl_redisplay();
	}
}
