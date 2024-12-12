/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 09:19:03 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/11 23:52:49 by tsururukako      ###   ########.fr       */
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

static char	*init_process(char *nptr, long long *sign)
{
	while (*nptr && ft_isspace(*nptr) == true)
		nptr++;
	if (*nptr == '-')
		(*sign) = -1;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	return (nptr);
}

// atoi_pointer return long num or NULL
long	*atol_pointer(char *nptr)
{
	long long	num;
	long		*ptr;
	long long	sign;

	num = 0;
	sign = 1;
	nptr = init_process(nptr, &sign);
	if (!ft_isdigit(*nptr))
		return (NULL);
	while (ft_isdigit(*nptr))
	{
		if (ft_isover(sign, num, (long)(*nptr - '0')))
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
