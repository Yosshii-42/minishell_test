#include "../minishell.h"

volatile sig_atomic_t g_sig_status = READLINE;

static int	dup_stdin(int *fd)
{
	*fd = dup(STDIN_FILENO);
	if (*fd == -1)
		return (ft_printf(2, "%s\n", strerror(errno)), FALSE);
	return (TRUE);
}

static int	do_minishell(t_env *env, char *line, char *pwd)
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
	if (*line)
	{
		add_history(line);
		dup_stdin(&origi_stdin);
		token = make_token_lst(line);//TODO !token時の処理
		if (!ft_memcmp(line, "clear", 6))
			clear_history();
		else
			status = run_process(token, path, pwd, &origi_stdin);
	}
	if (path)
		free_split(path);
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;
	char	*pwd;
	t_env	*ptr;

	init_signal();
	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
	if (!(env = set_env(envp)))
		exit(EXIT_FAILURE);
	rl_outstream = stdout;
	while (1)
	{
		ptr = env;
		pwd = getenv("PWD");
		if (!(line = readline("mçnishell$ ")) && ft_printf(1, "exit\n"))
			break ;
		else if (!ft_memcmp(line, "exit", 5))
			break;
		else if (!ft_memcmp(line, "env", 4))
		{
			while (ptr)
			{
				ft_printf(1, "%s=%s\n", ptr->key, ptr->value);
				if (ptr->next)
					ptr = ptr->next;
				else
					break;
			}
		}
		// else if (ft_memcmp(line, "unset", 6) == 32)
		// {
			// "unset"の次までpointerを進める
			// char **split;
			// split = (line, ' ');
			// while (ptr)
			// {
			// 	int i = -1;
			// 	while (split[++i])
			// 	{
			// 		if (!ft_memcmp(ptr->key, split[i], ft_strlen(split[i]) + 1))
			// 			ptr
			// 			if (ptr->next)
			// 				ptr->pre->next = ptr->next
			// 			else
			// 				ptr->pre->next = NULL
			// 			if (ptr->pre)
			// 				ptr->next->pre = ptr->pre
			// 			new_ptr = ptr->next
			// 			free(ptr)
			// 	}
				
			// }
		// }
		ft_printf(1, "status = %d\n", do_minishell(env, line, pwd));
		free(line);
	}
	free_env(env);
	clear_history();
	exit(0);
}
