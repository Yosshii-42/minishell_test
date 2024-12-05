/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:51 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/05 03:18:57 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_sig_status;

static void	dup_stdio(int *stdio)
{
	stdio[0] = -1;
	stdio[1] = -1;
	stdio[0] = safe_dup(STDIN_FILENO);
	stdio[1] = safe_dup(STDOUT_FILENO);
}

static void	clear_process(void)
{
	rl_clear_history();
	end_status(SET, 0);
}

static void	do_minishell(char *line)
{
	int			stdio[2];
	t_token		*token;
	int			command_count;

	add_history(line);
	dup_stdio(stdio);
	token = NULL;
	token = lexer(line);
	if (!token)
		return ;
	command_count = cmd_count(token);
	if (!ft_memcmp(line, "clear", 6))
		clear_process();
	else
		end_status(SET, run_process(token, stdio, command_count));
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;

	env = NULL;
	env = make_env(argc, argv, envp);
	if (!env)
		exit(EXIT_FAILURE);
	env = set_env(SET, env);
	rl_outstream = stdout;
	g_sig_status = 0;
	rl_event_hook = event;
	while (1)
	{
		init_signal();
		line = readline("minishell$ ");
		if (!line && ft_printf(1, "exit\n"))
			break ;
		set_line(SET, line);
		do_minishell(line);
		free(line);
	}
	free_env(set_env(GET, NULL));
	clear_history();
	exit(EXIT_SUCCESS);
}
