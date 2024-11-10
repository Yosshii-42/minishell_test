/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 05:45:20 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/09 01:59:26 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// bool    main_exit(char *line)
// // bool    builtin_exit(t_cmd *cmd)
// {
//     int result;

//     result = ft_memcmp(line, "exit", 5);
//     if (result == 0 || result == 9 || result == 10 || result == 32)
//         return (true);
//     else
//         return (false);
// 	if (cmd->cmd[1])
		
// }

// {
//     int result;

//     result = ft_memcmp(line, "exit", 5);
//     if (result == 0 || result == 9 || result == 10 || result == 32)
//         return (true);
//     else
//         return (false);
// }

void	print_env(t_env *env)
{
	while (env)
	{
		ft_printf(1, "%s=%s\n", env->key, env->value);
		if (env->next)
			env = env->next;
		else
			break ;
	}	
}

void	print_dolquestion(char *str, int status)
{
	ft_printf(2, "bash: ");
	ft_putnbr_fd(status, 2);
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
