/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:28:05 by yotsurud          #+#    #+#             */
/*   Updated: 2024/04/29 19:40:24 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr_base_fd(unsigned long nbr, char *base, int fd, int *count)
{
	unsigned long		base_len;

	base_len = (unsigned long)ft_strlen(base);
	if (nbr >= base_len)
		ft_putnbr_base_fd(nbr / base_len, base, fd, count);
	ft_putchar_fd(base[nbr % base_len], fd);
	(*count)++;
}

int	ft_select_conversion(int fd, va_list ap, const char character)
{
	int	count;

	count = 0;
	if (character == 'c')
		count = ft_put_c(fd, (char)va_arg(ap, int));
	else if (character == 's')
		count = ft_put_s(fd, (char *)va_arg(ap, char *));
	else if (character == 'p')
		count = ft_put_p(fd, (void *)va_arg(ap, void *));
	else if (character == 'd' || character == 'i')
		count = ft_put_di(fd, (int)va_arg(ap, int));
	else if (character == 'u' || character == 'x' || character == 'X')
		count = ft_put_uxx(fd, (unsigned int)va_arg(ap, unsigned int),
				character);
	else
		count = ft_put_c(fd, character);
	return (count);
}

int	ft_printf(int fd, const char *fmt, ...)
{
	va_list	ap;
	int		count;

	count = 0;
	if (!fmt)
		return (-1);
	va_start(ap, fmt);
	while (*fmt)
	{
		if (*fmt != '%')
			count += ft_put_c(fd, *fmt);
		else if (*fmt == '%' && *(fmt + 1))
			count += ft_select_conversion(fd, ap, *(++fmt));
		if (*fmt)
			fmt++;
	}
	va_end(ap);
	return (count);
}
