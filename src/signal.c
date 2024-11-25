/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:29:31 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/25 19:46:02 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_parent_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	reset_signal(int signum)
{
	struct sigaction	sa;

	if (sigemptyset(&sa.sa_mask) < 0)
		fatal_error_exit("failed to sigemptyset");
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error_exit("failed to sigaciton");
}

void	ignore_signal(int signum)
{
	struct sigaction	sa;

	if (sigemptyset(&sa.sa_mask) < 0)
		fatal_error_exit("failed to sigemptyset");
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error_exit("failed to sigaciton");
}

void	ready_signal(int signum)
{
	struct sigaction	sa;

	if (sigemptyset(&sa.sa_mask) < 0)
		fatal_error_exit("failed to sigemptyset");
	//sa.sa_flags = 0;
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = signal_handler;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error_exit("failed to sigaction");
}

void	init_signal(void)
{
	if (isatty(STDIN_FILENO))
		rl_event_hook = event;
	rl_catch_signals = 0;
	ignore_signal(SIGQUIT);
	ready_signal(SIGINT);
}

void	destroy_signal(void)
{
	reset_signal(SIGQUIT);
	reset_signal(SIGINT);
}
