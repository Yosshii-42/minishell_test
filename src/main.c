/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:51 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/09 01:59:12 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_sig_status = READLINE;

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
	add_history(line);
	dup_stdin(&origi_stdin);
	token = make_token_lst(line, status_num);
	if (!token)
		return (ft_printf(2, "bash: %s\n", strerror(errno)), EXIT_FAILURE);
	if (!ft_memcmp(line, "clear", 6))
		clear_history();
	else
		status = run_process(token, path, pwd, &origi_stdin);
	if (path)
		free_split(path);
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;
	char	*pwd;
	int		status;

	init_signal();
	env = set_env(argc, argv, envp, &status);
	if (!env)
		exit(EXIT_FAILURE);
	rl_outstream = stdout;
	while (1)
	{
		pwd = getenv("PWD");
		line = readline("minishell$ ");
		if (!line && ft_printf(1, "exit\n"))
			break ;
		else if (builtin_exit(line) == true)
			break ;
		else if (builtin(line, env, &status) == true)
			status = do_minishell(env, line, pwd, status);
		free(line);
	}
	free_env(env);
	clear_history();
	exit(0);
}
