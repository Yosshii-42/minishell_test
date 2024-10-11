/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:18:41 by yotsurud          #+#    #+#             */
/*   Updated: 2024/04/23 13:32:59 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_cal_digit(int num)
{
	int	i;

	i = 0;
	while (num)
	{
		i++;
		num /= 10;
	}
	return (i);
}

void	ft_setnbr(long n, char *buf, int i)
{
	if (n >= 10)
		ft_setnbr(n / 10, buf, i - 1);
	buf[i] = n % 10 + '0';
}

char	*ft_itoa(int n)
{
	int		digit;
	int		is_0orless;
	char	*buf;
	long	num;

	digit = ft_cal_digit(n);
	is_0orless = 0;
	num = (long)n;
	if (num <= 0)
		is_0orless = 1;
	buf = (char *)ft_calloc((digit + is_0orless + 1), sizeof(char));
	if (!buf)
		return (NULL);
	if (num <= 0)
	{
		buf[0] = '-';
		num = -num;
	}
	ft_setnbr(num, buf, digit + is_0orless - 1);
	buf[digit + is_0orless] = '\0';
	return (buf);
}	
