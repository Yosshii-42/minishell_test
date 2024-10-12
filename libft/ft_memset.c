/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:19:51 by yotsurud          #+#    #+#             */
/*   Updated: 2024/04/17 18:04:18 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*tmp;
	long	i;

	tmp = (char *)s;
	i = 0;
	while (i < (long)n)
	{
		tmp[i] = c;
		i++;
	}
	return (tmp);
}
