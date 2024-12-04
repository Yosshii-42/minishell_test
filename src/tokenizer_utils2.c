/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:41:10 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/04 19:32:59 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin_one(char *str, char c)
{
	char	join[2];

	join[0] = c;
	join[1] = '\0';
	return (strjoin_with_free(str, join, FREE_S1));
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
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}
