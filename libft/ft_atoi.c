/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:05:17 by yotsurud          #+#    #+#             */
/*   Updated: 2024/04/29 13:49:21 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isover(long sign, long num, long next_num)
{
	if (num < LONG_MAX / 10)
		return (FALSE);
	if (num > LONG_MAX / 10)
		return (TRUE);
	if (sign == 1 && next_num > LONG_MAX % 10)
		return (TRUE);
	else if (sign == -1 && next_num > (LONG_MIN % 10) * (-1))
		return (TRUE);
	else
		return (FALSE);
}

int	ft_atoi(const char *nptr)
{
	long	num;
	long	sign;

	num = 0;
	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-')
		sign = -1;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	while (ft_isdigit(*nptr))
	{
		if (sign == 1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return ((int)LONG_MAX);
		else if (sign == -1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return ((int)LONG_MIN);
		num = num * 10 + *nptr++ - '0';
	}
	return ((int)(sign * num));
}
