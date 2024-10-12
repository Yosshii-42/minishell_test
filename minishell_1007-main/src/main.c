#include "../minishell.h"

static void	initial_error(int argc, char **argv)
{
	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
}

static void	handle_sigint(int sig)
{
	(void)sig;

	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void close_extra_fds(void) {
    for (int fd = 3; fd < 1024; fd++) {
        close(fd);  // 不要なファイルディスクリプタを閉じる
    }
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;

    close_extra_fds();  // シェル起動時に余分なFDを閉じる
	signal(SIGINT, handle_sigint);
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
