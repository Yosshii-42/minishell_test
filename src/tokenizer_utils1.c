/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:24:10 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/01 16:05:22 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*space_skip(char *input)
{
	while (*input && ft_isspace(*input))
		input++;
	return (input);
}

int	count_word_len(char *input, t_token *new)
{
	int	len;

	len = 0;
	while (*(input + len) && !ft_isspace(*(input + len)) && \
		!ft_strchr(SPECIAL_TOKEN, *(input + len)))
	{
		if (*(input + len) == '\'')
			new->is_quoted = true;
		else if (*(input + len) =='\"')
			new->is_double_quoted = true;
		else if (*(input + len) == '$')
			new->is_dollar = true;
		len++;
	}
	return (len);
}

int	count_meta_len(char *line)
{
	if (line[0] == '<' && line[1] == '<')
		return (2);
	if (line[0] == '>' && line[1] == '>')
		return (2);
	return (1);
}

int	count_envname_len(char *input, t_token *new, int start)
{
	int	len;

	(void)start;
	len = 0;
	if (!(*input))
		return (0);
	//if (input[i] == '?')
	//	return (1);
	//while (ft_isalnum(token[i + len]) || token[i + len] == '_')
	while (ft_isspace(*(input+ len) == false))
	{
		if (*(input + len) == '\'')
			new->is_quoted = true;
		else if (*(input + len) =='\"')
			new->is_double_quoted = true;
		len++;
	}
	return (len);
}
