/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:21:56 by yotsurud          #+#    #+#             */
/*   Updated: 2024/07/19 19:45:34 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s1_cp;
	char	*s2_cp;
	char	*tmp;
	char	*result;
	char	*s1_tmp;

	if (!s1 || !s2)
		return (NULL);
	s1_cp = (char *)s1;
	s2_cp = (char *)s2;
	s1_tmp = (char *)s1;
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
