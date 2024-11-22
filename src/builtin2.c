/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:55:33 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/20 16:01:49 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_unset(t_cmd *cmd, t_env **env)
{
	if (!cmd->cmd[1])
		return (EXIT_SUCCESS);
	while ((*env))
	{
		if (!ft_memcmp((*env)->key, cmd->cmd[1], ft_strlen(cmd->cmd[1]) + 1))
		{
			if ((*env)->pre)
				(*env)->pre->next = (*env)->next;
			else
			{
				if ((*env)->next)
				{
					(*env)->next->pre = NULL;
					env = &(*env)->next;
				}
			}
			return (free((*env)->key), free((*env)->value), free(*env), 0);
		}
		*env = (*env)->next;
	}
	return (EXIT_SUCCESS);
}

int	builtin_env(t_env *env)
{
	while (env)
	{
		ft_printf(1, "%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}

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
	if (*nptr == '-')
		sign = -1;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	while (ft_isdigit(*nptr))
	{
		if (sign == 1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return (NULL);
		else if (sign == -1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return (NULL);
		num = num * 10 + *nptr++ - '0';
	}
	num = sign * num;
	ptr = (long *)&num;
	if (*nptr && !ft_isdigit(*nptr))
		return (NULL);
	else
		return (ptr);
}

int	builtin_exit(t_cmd *cmd)
{
	int		count;
	long	*result;

	count = 0;
	while (cmd->cmd[count])
		count++;
	result = NULL;
	if (count == 1)
	{
		free_cmd(cmd);
		exit (0);
	}
	else if (count >= 3)
	{
		ft_printf(2, "exit\nbash: exit: too many arguments\n");
		free_cmd(cmd);
		exit (EXIT_FAILURE);
	}
	else
	{
		result = atol_pointer(cmd->cmd[1]);
		printf("result = %ld\n", *result);
		if (result == NULL)
		{
			ft_printf(2, "bash: exit: %s", cmd->cmd[1]);
			ft_printf(2, ": numeric argument rewuired\n");
			free_cmd(cmd);
			exit (2);
		}
		else
		{
			free_cmd(cmd);
			printf("exit result = %ld\n", *result);
			exit ((*result) % 256);
		}
	}
}
