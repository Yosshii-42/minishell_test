/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:21:31 by yotsurud          #+#    #+#             */
/*   Updated: 2024/04/29 18:43:26 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	size_t			len;
	unsigned char	cc;

	i = 0;
	len = 0;
	cc = (unsigned char)c;
	len = ft_strlen(s);
	while (s[i] != cc && i < len)
		i++;
	if (s[i] == cc && i <= len)
		return ((char *)&s[i]);
	else
		return (NULL);
}
