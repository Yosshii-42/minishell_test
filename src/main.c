#include "../minishell.h"

volatile sig_atomic_t g_sig_status = READLINE;

static int	dup_stdin(int *fd)
{
	*fd = dup(STDIN_FILENO);
	if (*fd == -1)
		return (ft_printf(2, "%s\n", strerror(errno)), FALSE);
	return (TRUE);
}

static int	do_minishell(t_env *env, char *line)
{
	char	**path;
	char	*pwd;
	int		original_stdin;
	int		status;

	original_stdin = 0;
	status = 0;
	path = NULL;
	if (getenv_str(env, "PATH"))
		path = ft_split(getenv_str(env, "PATH"), ':');
	pwd = NULL;
	pwd = getenv_str(env, "PWD");
	if (*line)
	{
		add_history(line);
		dup_stdin(&original_stdin);
		if (!ft_memcmp(line, "clear", 6))
			clear_history();
		else
			status = run_process(line, path, pwd, &original_stdin);
	}
	if (path)
		free_split(path);
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;

	init_signal();
	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
	env = set_env(envp);
	if (!env)
		exit(EXIT_FAILURE);
	rl_outstream = stdout;
	while (1)
	{
		if (!(line = readline("minishell$ ")))
		{
			ft_printf(1, "exit\n");
			break ;
		}
		ft_printf(1, "status = %d\n", do_minishell(env, line));
	}
	free_env(env);
	clear_history();
	exit(0);
}
