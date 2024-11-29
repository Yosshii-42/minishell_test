/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:24:10 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/29 18:27:08 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*space_skip(char *input)
{
	while (*input && ft_isspace(*input))
		input++;
	return (input);
}

int	count_meta_len(char *line)
{
	if (line[0] == '<' && line[1] == '<')
		return (2);
	if (line[0] == '>' && line[1] == '>')
		return (2);
	return (1);
}

int	count_envname_len(char *token, int i)
{
	int	len;

	len = 0;
	if (!token[i])
		return (0);
	if (token[i] == '?')
		return (1);
	while (ft_isalnum(token[i + len]) || token[i + len] == '_')
		len++;
	return (len);
}
