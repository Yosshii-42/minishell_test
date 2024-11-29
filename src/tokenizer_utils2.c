/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:41:10 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/29 16:23:37 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	char	*d;

	d = dest;
	while (*src)
		*d++ = *src++;
	*d = '\0';
	return (dest);
}

char	*ft_strjoin_one(char *str, char c)
{
	char	*new_str;
	size_t	len;

	len = 0;
	if (str)
		len = ft_strlen(str);
	new_str = (char *)safe_malloc(len + 2, sizeof(char));
	ft_strcpy(new_str, str);
	free(str);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || \
	c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}
