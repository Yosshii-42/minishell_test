/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:05:17 by yotsurud          #+#    #+#             */
/*   Updated: 2024/07/03 19:23:52 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isover(long long sign, long long num, long long next_num)
{
	if (num < LLONG_MAX / 10)
		return (FALSE);
	if (num > LLONG_MAX / 10)
		return (TRUE);
	if (sign == 1 && next_num > LLONG_MAX % 10)
		return (TRUE);
	else if (sign == -1 && next_num > (LLONG_MIN % 10) * (-1))
		return (TRUE);
	else
		return (FALSE);
}

long	ft_atol(const char *nptr)
{
	long long	num;
	long long	sign;

	num = 0;
	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-')
		sign = -1;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	if (!ft_isdigit(*nptr))
		return ((long)LLONG_MAX);
	while (ft_isdigit(*nptr))
	{
		if (sign == 1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return ((long)LLONG_MAX);
		else if (sign == -1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return ((long)LLONG_MIN);
		num = num * 10 + *nptr++ - '0';
	}
	return ((long)(sign * num));
}
