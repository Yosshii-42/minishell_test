/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:29:40 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/27 10:15:32 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*strjoin_with_free(char *s1, char *s2, int select)
{
	int		i;
	int		j;
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	i = -1;
	j = -1;
	if (!(*s1))
		result = (char *)safe_malloc(ft_strlen(s2) + 1, sizeof(char));
	else
		result = (char *)safe_malloc(ft_strlen(s1) + ft_strlen(s2) + 1
				, sizeof(char));
	while (s1[++i])
		result[i] = s1[i];
	while (s2[++j])
		result[i + j] = s2[j];
	result[i + j] = '\0';
	if (select == 1 && *s1)
		free(s1);
	else if (select == 2 && *s2)
		free(s2);
	return (result);
}

size_t	strchr_len(const char *s, int c)
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
		return (i);
	else
		return (0);
}
