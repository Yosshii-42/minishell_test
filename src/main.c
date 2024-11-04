#include "../minishell.h"

volatile sig_atomic_t g_sig_status = READLINE;

static int	dup_stdin(int *fd)
{
	*fd = dup(STDIN_FILENO);
	if (*fd == -1)
		return (ft_printf(2, "%s\n", strerror(errno)), FALSE);
	return (TRUE);
}

static int	do_minishell(t_env *env, char *line, char *pwd, int status_num)
{
	char	**path;
	int		origi_stdin;
	int		status;
	t_token	*token;

	origi_stdin = 0;
	status = 0;
	path = NULL;
	if (getenv_str(env, "PATH"))
		path = ft_split(getenv_str(env, "PATH"), ':');
	// if (*line)
	// {
		add_history(line);
		dup_stdin(&origi_stdin);
		token = make_token_lst(line, status_num);//TODO !token時の処理
		if (!ft_memcmp(line, "clear", 6))
			clear_history();
		else
			status = run_process(token, path, pwd, &origi_stdin);
	// }
	if (path)
		free_split(path);
	return (status);
}

void	print_env(t_env *env)
{
	while (env)
	{
		ft_printf(1, "%s=%s\n", env->key, env->value);
		if (env->next)
			env = env->next;
		else
			break;
	}	
}

void	print_dolquestion(char *str, int status)
{
	ft_printf(2, "bash: ");
	ft_putnbr_fd(status, 2);
	if (*str)
		ft_printf(2, "%s", str);
		ft_printf(2, ": command not found\n", str);
}

bool	builtin(char *line, t_env *env, int *status)
{
	if (ft_memcmp(line, "env", 4) == 0)
		return (print_env(env), false);
	else if (!ft_strncmp(line, "$\?", 2))
	{
		print_dolquestion(ft_strchr(line, '?') + 1, *status);
		return (*status = 127, false);
	}
	else
		return (true);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;
	char	*pwd;
	int		status;

	init_signal();
	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
	if (!(env = set_env(envp)))
		exit(EXIT_FAILURE);
	rl_outstream = stdout;
	status = 0;
	while (1)
	{
		pwd = getenv("PWD");
		if (!(line = readline("mnishell$ ")) && ft_printf(1, "exit\n"))
			break ;
		else if (ft_memcmp(line, "exit", 5) == 0)
			break;
		else if (builtin(line, env, &status) == true)
			status = do_minishell(env, line, pwd, status);
		free(line);
	}
	free_env(env);
	clear_history();
	exit(0);
}
