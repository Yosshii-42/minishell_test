/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:51 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/24 21:45:01 by hurabe           ###   ########.fr       */
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

static void	do_minishell(t_env *env, char *line)
{
	int			stdio[2];
	t_token		*token;

	stdio[0] = -1;
	stdio[1] = -1;
	add_history(line);
	if (dup_stdio(stdio) == false)
	{
		end_status(SET, EXIT_FAILURE);
		return ;
	}
	token = NULL;
	token = make_token_lst(line);
	if (!ft_memcmp(line, "clear", 6))
	{
		rl_clear_history();
		end_status(SET, 0);
	}
	else
		end_status(SET, run_process(token, env, stdio));
	printf("status = %d\n", end_status(GET, 0));
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;

	init_signal(); // シェル全体のシグナルを初期化する
	env = set_env(argc, argv, envp);
	if (!env)
		exit(EXIT_FAILURE);
	rl_outstream = stdout;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line && ft_printf(1, "exit\n"))
			break ;
		do_minishell(env, line);
		free(line);
	}
	free_env(env);
	clear_history();
	exit(EXIT_SUCCESS);
}

int end_status(int type, int end_status)
{
	static int e_status;

	if (type == SET)
		e_status = end_status;
	return (e_status);
}
