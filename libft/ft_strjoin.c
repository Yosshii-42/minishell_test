/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:21:56 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/26 00:33:05 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s1_cp;
	char	*s2_cp;
	char	*tmp;
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	s1_cp = (char *)s1;
	s2_cp = (char *)s2;
	if (!(*s1))
		tmp = (char *)ft_calloc(ft_strlen(s2) + 1, sizeof(char));
	else
		tmp = (char *)ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1,
				sizeof(char));
	if (!tmp)
		return (NULL);
	result = tmp;
	while (*s1_cp)
		*(tmp++) = *(s1_cp++);
	while (*s2_cp)
		*(tmp++) = *(s2_cp++);
	return (result);
}
