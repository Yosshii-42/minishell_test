/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:51 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/25 21:51:19 by hurabe           ###   ########.fr       */
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

static void	do_minishell(char *line)
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
	// token = make_token_lst(line);
	token = lexer(line);
	if (!ft_memcmp(line, "clear", 6))
	{
		rl_clear_history();
		end_status(SET, 0);
	}
	else
		end_status(SET, run_process(token, stdio));
	ready_signal(SIGINT);
	printf("status = %d\n", end_status(GET, 0));
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;

	//init_signal();
	env = NULL;
	env = set_env(argc, argv, envp);
	if (!env)
		exit(EXIT_FAILURE);
	env = set_get_env(SET, env);
	rl_outstream = stdout;
	while (1)
	{
		init_signal();
		line = readline("minishell$ ");
		if (!line && ft_printf(1, "exit\n"))
			break ;
		do_minishell(line);
		free(line);
	}
	free_env(set_get_env(GET, NULL));
	clear_history();
	exit(EXIT_SUCCESS);
}

int end_status(int type, int end_status)
{
	static int	status;

	if (type == SET)
		status = end_status;
	return (status);
}

t_env	*set_get_env(int type, t_env *new_env)
{
	static t_env	*env;

	if (type == SET)
		env = new_env;
	return (env);
}