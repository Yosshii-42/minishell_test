/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:51 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/26 01:30:24 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_sig_status = READLINE;

static int	dup_stdio(int *stdio)
{
	stdio[0] = -1;
	stdio[1] = -1;
	stdio[0] = dup(STDIN_FILENO);
	stdio[1] = dup(STDOUT_FILENO);
	if (stdio[0] < 0 || stdio[1] < 0)
		return (ft_printf(2, "%s\n", strerror(errno)), false);
	return (true);
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
	if (dup_stdio(stdio) == false)
		exit(EXIT_FAILURE);
	token = NULL;
	token = lexer(line);
	set_token(SET, token);
	command_count = cmd_count(token);
	if (!ft_memcmp(line, "clear", 6))
		clear_process();
	else
		end_status(SET, run_process(token, stdio, command_count));
	ready_signal(SIGINT);
	printf("status = %d\n", end_status(GET, 0));
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
	while (1)
	{
		init_signal();
		line = readline("minishell$ ");
		if (!line && ft_printf(1, "exit\n"))
			break ;
		do_minishell(line);
		free(line);
	}
	free_env(set_env(GET, NULL));
	clear_history();
	exit(EXIT_SUCCESS);
}
