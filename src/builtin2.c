/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:55:33 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/25 23:43:13 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_unset(t_cmd *cmd, t_env **env)
{
	if (!cmd->cmd[1])
		end_status(SET, EXIT_SUCCESS);
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
			free((*env)->key);
			free((*env)->value);
			free(*env);
			end_status(SET, EXIT_SUCCESS);
		}
		*env = (*env)->next;
	}
	end_status(SET, EXIT_SUCCESS);
}

// void	builtin_env(t_env *env)
void	builtin_env(void)
{
	t_env	*env;

	env = set_env(GET, NULL);
	while (env)
	{
		if (env->value)
		{
			ft_printf(1, "%s=", env->key);
			if (*(env->value))
				ft_printf(1, "%s", env->value);
			ft_printf(1, "\n");
		}
		env = env->next;
	}
	end_status(SET, EXIT_SUCCESS);
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

void	builtin_exit(t_cmd *cmd)
{
	int		count;
	long	*result;

	count = 0;
	while (cmd->cmd[count])
		count++;
	result = NULL;
	if (count == 1)
		end_status(SET, EXIT_SUCCESS);
	else if (count >= 3)
	{
		ft_printf(2, "exit\nbash: exit: too many arguments\n");
		end_status(SET, 2);
	}
	else
	{
		result = atol_pointer(cmd->cmd[1]);
		if (result == NULL)
		{
			ft_printf(2, "bash: exit: %s", cmd->cmd[1]);
			ft_printf(2, ": numeric argument rewuired\n");
			end_status(SET, EXIT_FAILURE);
		}
		else
		{
	printf("result = %ld\n", *result);
			printf("exit result = %d\n", end_status(GET, 0));
			end_status(SET, (*result) % 256);
		}
	}
}
