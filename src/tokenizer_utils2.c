/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:41:10 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/07 01:04:52 by tsururukako      ###   ########.fr       */
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

// bool	is_quote(char c)
// {
// 	return (c == '\'' || c == '\"');
// }

void	set_kind_as_command(t_token *token, int *command_flag)
{
	token->kind = COMMAND;
	(*command_flag)++;
}

void	init_variables(int *i, int *single_flag, int *double_flag)
{
	*i = 0;
	*single_flag = 0;
	*double_flag = 0;
}
