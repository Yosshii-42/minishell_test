/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_and_count.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 13:37:43 by yotsurud          #+#    #+#             */
/*   Updated: 2024/05/10 11:53:18 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_put_c(int fd, int c)
{
	ft_putchar_fd(c, fd);
	return (1);
}

int	ft_put_s(int fd, char *str)
{
	int	count;

	count = 0;
	if (!str)
		return (write(fd, "(null)", 6), 6);
	while (*str)
	{
		write(fd, str++, 1);
		count++;
	}
	return (count);
}

int	ft_put_p(int fd, void *ptr)
{
	int				count;
	unsigned long	nbr;

	nbr = (unsigned long)ptr;
	ft_putstr_fd("0x", fd);
	count = 2;
	ft_putnbr_base_fd(nbr, HEX_LOW, fd, &count);
	return (count);
}

int	ft_put_di(int fd, int nbr)
{
	int		count;
	long	num;

	count = 0;
	num = (long)nbr;
	if (num < 0)
	{
		ft_putchar_fd('-', fd);
		num = -num;
		count++;
	}
	ft_putnbr_base_fd((unsigned int)num, DECIMAL, fd, &count);
	return (count);
}

int	ft_put_uxx(int fd, unsigned int nbr, char character)
{
	int	count;

	count = 0;
	if (character == 'u')
		ft_putnbr_base_fd(nbr, DECIMAL, fd, &count);
	if (character == 'x')
		ft_putnbr_base_fd(nbr, HEX_LOW, fd, &count);
	if (character == 'X')
		ft_putnbr_base_fd(nbr, HEX_UP, fd, &count);
	return (count);
}
