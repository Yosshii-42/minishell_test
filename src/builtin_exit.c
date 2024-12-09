/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:55:33 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/05 19:39:37 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	ft_isover(long long sign, long long num, long long next_num)
{
	if (num < LLONG_MAX / 10)
		return (false);
	if (num > LLONG_MAX / 10)
		return (true);
	if (sign == 1 && next_num > LLONG_MAX % 10)
		return (true);
	else if (sign == -1 && next_num > (LLONG_MIN % 10) * (-1))
		return (true);
	else
		return (false);
}

// atoi_pointer return long num or NULL
static long	*atol_pointer(const char *nptr)
{
	long long	num;
	long		*ptr;
	long long	sign;

	num = 0;
	sign = 1;
	while (*nptr && ft_isspace(*nptr) == true)
		nptr++;
	if (*nptr == '-')
		sign = -1;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	if (!ft_isdigit(*nptr))
		return (NULL);
	while (ft_isdigit(*nptr))
	{
		if (sign == 1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return (NULL);
		else if (sign == -1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return (NULL);
		num = num * 10 + *nptr++ - '0';
	}
	while (*nptr)
	{
		if (*nptr == ' ' || *nptr == '\t')
			nptr++;
		else
			return (NULL);
	}
	num = sign * num;
	ptr = (long *)&num;
	return (ptr);
}

void	print_err_and_set_exit_status(char *argument, long *result)
{
	if (result == NULL)
	{
		ft_printf(2, "bash: exit: %s", argument);
		ft_printf(2, ": numeric argument rewuired\n");
		end_status(SET, EXIT_FAILURE);
	}
	else
		end_status(SET, (*result) % 256);
}

void	numeric_error(char *argument)
{
	ft_printf(2, "bash: exit: %s", argument);
	ft_printf(2, ": numeric argument rewuired\n");
	end_status(SET, 2);
}

void	too_many_error(t_cmd *cmd)
{
	ft_printf(2, "bash: exit: too many arguments\n");
	end_status(SET, 1);
	cmd->pipe_flag = -1;
}

bool	builtin_exit(t_cmd *cmd)
{
	int		count;
	long	*result;

	count = -1;
	while (cmd->cmd[++count])
		;
	if (count == 1)
	{
		end_status(SET, EXIT_SUCCESS);
		return (true);
	}
	result = atol_pointer(cmd->cmd[1]);
	if (count == 2 && !result)
		numeric_error(cmd->cmd[1]);
	else if (count == 2)
		end_status(SET, *result);
	else if (count >= 3 && result)
		too_many_error(cmd);
	else
		numeric_error(cmd->cmd[1]);
	return (true);
}
