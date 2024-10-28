#include "../minishell.h"

volatile sig_atomic_t g_sig_status = READLINE;

static void	init(int argc, char **argv)
{
	int	fd;

	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
	fd = 3;
	while (fd < 1024)
		close(fd++);
	
}

//static void	handle_sigint(int sig)
//{
//	(void)sig;

<<<<<<< HEAD
	rl_on_new_line();
    // rl_replace_line("", 0);
	rl_redisplay();
}
=======
//	rl_on_new_line();
//    rl_replace_line("", 0);
//	rl_redisplay();
//}
>>>>>>> dev_urabe

void	dup_stdin(int *fd)
{
	*fd = dup(STDIN_FILENO);
	if (*fd == -1)
		print_error_and_exit(strerror(errno));
}

void	close_duped_stdin(int *fd)
{
	if (dup2(*fd, STDIN_FILENO) == -1)
		print_error_and_exit(strerror(errno));
	close(*fd);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;
	int original_stdin_fd;

	original_stdin_fd = 0;
	init_signal();
	init(argc, argv);
	env = set_env(envp);
	rl_outstream = stdout;
	while (1)
	{
		dup_stdin(&original_stdin_fd);
		//if (!(line = readline("minishell$ ")))
		//	break ;
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
		{
			add_history(line);
			if (!ft_memcmp(line, "clear", 6))
				clear_history();
			else
				run_process(line, env);
		}
		close_duped_stdin(&original_stdin_fd);
	}
	clear_history();
	exit(0);
}
