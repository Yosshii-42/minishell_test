#include "../minishell.h"

static void	init(int argc, char **argv)
{
	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
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

    close_extra_fds();  // シェル起動時に余分なFDを閉じる
	// signal(SIGINT, handle_sigint);
	setup_sigint_handler();
	// setup_sigchld_handler();
	init(argc, argv);
	env = set_env(envp);
	rl_outstream = stdout;
	while (1)
	{
		if (!(line = readline("minishell$ ")))
			break ;
		if (*line)
		{
			add_history(line);
			if (!ft_memcmp(line, "clear", 6))
				clear_history();
			//if () シグナルを受信したら
			// rl_on_new_line(); 新しい行に移ったことを明示
			// rl_redisplay(); 　プロンプトを再表示
			else
				run_process(line, env);
			if (line)
				free(line);
		}
	}
	clear_history();
	exit(0);
}
