/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:18:41 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/27 01:22:45 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../src/minishell.h"

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
	buf = NULL;
	buf = (char *)safe_malloc((digit + is_0orless + 1), sizeof(char));
	if (num <= 0)
	{
		buf[0] = '-';
		num = -num;
	}
	ft_setnbr(num, buf, digit + is_0orless - 1);
	buf[digit + is_0orless] = '\0';
	return (buf);
}	
