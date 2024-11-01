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
		write(STDERR_FILENO, "^C\n", 3);
	}
	else
	{
		g_sig_status = SIG_INT;
		write(STDOUT_FILENO, "^C\n", 3);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGPIPE)
			return (0);
		else
			return (128 + WTERMSIG(status));
	}
	return (status);
}
