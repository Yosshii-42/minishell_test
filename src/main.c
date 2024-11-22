/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:51 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/09 17:58:26 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_sig_status = READLINE;

static int	dup_stdio(int *stdio)
{
	stdio[0] = dup(STDIN_FILENO);
	stdio[1] = dup(STDOUT_FILENO);
	if (stdio[0] < 0 || stdio[1] < 0)
		return (ft_printf(2, "%s\n", strerror(errno)), false);
	return (true);
}

static int	do_minishell(t_env *env, char *line, int status_num)
{
	char	**path;
	int		stdio[2];
	int		status;
	t_token	*token;

	stdio[0] = -1;
	stdio[1] = -1;
	status = 0;
	path = NULL;
	if (getenv_str(env, "PATH"))
		path = ft_split(getenv_str(env, "PATH"), ':');
	if (dup_stdio(stdio) == false)
		return (EXIT_FAILURE);
	token = make_token_lst(line, status_num);
	// if (!token) //malloc errorをどこかで出す
	// 	return (ft_printf(2, "bash: %s\n", strerror(errno)), EXIT_FAILURE);
	if (!ft_memcmp(line, "clear", 6))
		clear_history();
	else
		status = run_process(token, env, path, stdio);
	if (path)
		free_split(path);
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;
	int		status;

	init_signal();
	status = 0;
	env = set_env(argc, argv, envp, &status);
	if (!env)
		exit(EXIT_FAILURE);
	rl_outstream = stdout;
	while (1)
	{
		line = readline("minishell$ ");
		add_history(line);
		if (print_dolquestion(line, &status) == true)
			status = do_minishell(env, line, status);
		free(line);
	}
	free_env(env);
	clear_history();
	exit(EXIT_SUCCESS);
}
