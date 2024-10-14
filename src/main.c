#include "../minishell.h"
#include <asm-generic/signal-defs.h>

static void	initial_error(int argc, char **argv)
{
	int	fd;

	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
	fd = 3;
	while (fd < 1024)
		close(fd++);
}

static void	handle_sigint(int sig)
{
	(void)sig;

	rl_on_new_line();
	rl_redisplay();
}

void	setup_sigint_handler(void)
{
	struct sigaction sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;

	// signal(SIGINT, handle_sigint);
	setup_sigint_handler();
	initial_error(argc, argv);
	env = set_env(envp);
	rl_outstream = stderr;
	while (1)
	{
		if (!(line = readline("minishell$ ")))
			break ;
		if (*line)
		{
			add_history(line);
			if (!ft_memcmp(line, "clear", 6))
				rl_clear_history();
			//if () シグナルを受信したら
			// rl_on_new_line(); 新しい行に移ったことを明示
			// rl_redisplay(); 　プロンプトを再表示
			else
				run_process(line, env);
			free(line);
		}
	}
	rl_clear_history();
	exit(0);
}
