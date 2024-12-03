/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_token.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:44:28 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/03 17:40:10 by hurabe           ###   ########.fr       */
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
