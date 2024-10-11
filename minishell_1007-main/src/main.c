#include "../minishell.h"

static void	initial_error(int argc, char **argv)
{
	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;

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
				clear_history();
			//if () シグナルを受信したら
			// rl_on_new_line(); 新しい行に移ったことを明示
			// rl_redisplay(); 　プロンプトを再表示
			else
				run_process(line, env);
			free(line);
		}
	}
	exit(0);
}
