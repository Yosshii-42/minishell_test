/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:54:06 by yotsurud          #+#    #+#             */
/*   Updated: 2024/10/03 21:10:23 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../minishell.h"

static int	ft_count_i(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static char	*ft_make_string_i(char const *s, char c)
{
	size_t	j;
	size_t	len;
	char	*str;

	len = 0;
	while (s[len] && (s[len] != c))
		len++;
	str = (char *)ft_calloc((len + 1), sizeof(char));
	if (!str)
		return (NULL);
	j = -1;
	while (++j < len)
		str[j] = s[j];
	str[j] = '\0';
	return (str);
}

static void	*free_split(char **result, int i)
{
	while (result[i])
		free(result[i--]);
	if (result)
		free(result);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**result;

	if (!s)
		print_error_and_exit(strerror(errno));
	result = safe_malloc(ft_count_i(s, c) + 1, sizeof(char *));
	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			result[i] = ft_make_string_i(s, c);
			if (!result[i])
			{
				free_split(result, i);
				print_error_and_exit(strerror(errno));
			}
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	return (result);
}
