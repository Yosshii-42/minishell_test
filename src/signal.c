#include "../minishell.h"

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
	sa.sa_flags = 0;
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
