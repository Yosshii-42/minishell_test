/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:21:43 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/26 22:55:45 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../minishell.h"

char	*ft_strdup(const char *s1)
{
	int		len;
	int		i;
	char	*ptr;

	if (s1 == NULL)
	{
		ptr = (char *)safe_malloc(1, sizeof(char));
		return  (ptr[0] = '\0', ptr);
	}
	if (*s1 == '\0')
		len = 0;
	else
		len = ft_strlen(s1);
	ptr = NULL;
	ptr = (char *)safe_malloc(len + 1, sizeof(char));
	i = 0;
	while (i < len)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
