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
	if (g_sig_status == HEREDOC)
	{
		g_sig_status = SIG_INT;
		rl_replace_line("", 0);
		rl_done = 1;
	}
	else if (g_sig_status == IN_CMD)
	{
		g_sig_status = SIG_INT;
		write(STDERR_FILENO, "\n", 1);
	}
	else
	{
		g_sig_status = SIG_INT;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
