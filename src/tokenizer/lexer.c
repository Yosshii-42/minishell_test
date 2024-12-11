/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_lexer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:35:00 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/11 18:13:21 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	add_token_kind(t_token *token)
{
	int	commnad_flag;

	commnad_flag = 0;
	while (token)
	{
		if (*(token->word) == '|')
		{
			token = add_kind_pipe(token);
			commnad_flag = 0;
		}
		else if (*(token->word) == '<')
			token = add_kind_lessthan(token);
		else if (*(token->word) == '>')
			token = add_kind_morethan(token);
		else if (check_builtin(token->word) >= 0)
			token->kind = BUILTIN;
		else
			token = add_command_kind(token, commnad_flag);
		if (token->kind == BUILTIN || token->kind == COMMAND)
			commnad_flag++;
		token = token->next;
	}
}

bool	find_not_suport(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '$' && line[i + 1] && line[i + 1] == '$')
			return (ft_printf(2, "$$: unsuported\n"), false);
	}
	return (true);
}

t_token	*lexer(char *line)
{
	t_token	*token;
	t_token	*ptr;

	if (!find_syntax_error(line) || !find_not_suport(line))
		return (end_status(SET, 1), NULL);
	token = NULL;
	token = tokenizer(line);
	set_token(SET, token);
	ptr = token;
	while (ptr)
	{
		if (ft_strchr(ptr->word, '$'))
			ptr->word = expand_dollar(ptr->word, ptr);
		ptr = ptr->next;
	}
	ptr = token;
	while (ptr)
	{
		if (ft_strchr(ptr->word, '\'') || ft_strchr(ptr->word, '\"'))
			ptr->word = expand_quote(ptr->word, ptr);
		ptr = ptr->next;
	}
	add_token_kind(token);
	return (token);
}
