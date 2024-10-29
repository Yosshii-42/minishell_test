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

static int	dup_stdin(int *fd)
{
	*fd = dup(STDIN_FILENO);
	if (*fd == -1)
		return (ft_printf(2, "%s\n", strerror(errno)), FALSE);
	return (TRUE);
}

static int	close_duped_fd(int *fd)
{
	if (dup2(*fd, STDIN_FILENO) == -1)
		return (ft_printf(2, "%s\n", strerror(errno)), FALSE);
	return (close(*fd), TRUE);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;
	int original_stdin_fd;
	char	**path;
	char	*pwd;

	original_stdin_fd = 0;
	init_signal();
	init(argc, argv);
	env = set_env(envp);
	rl_outstream = stdout;
	while (1)
	{
		//if (!(line = readline("minishell$ ")))
		//	break ;
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		path = NULL;
		if (getenv_str(env, "PATH"))
			path = ft_split(getenv_str(env, "PATH"), ':');
		pwd = NULL;
		pwd = getenv_str(env, "PWD");
		if (*line && dup_stdin(&original_stdin_fd))
		{
			add_history(line);
			if (!ft_memcmp(line, "clear", 6))
				clear_history();
			else
				run_process(line, path, pwd);
			close_duped_fd(&original_stdin_fd);
		}
		free(line);
	}
	if (path)
		free_split(path);
	free_env(env);
	clear_history();
	exit(0);
}
