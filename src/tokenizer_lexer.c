/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_lexer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:35:00 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/05 03:45:00 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	add_token_kind(t_token *token)
{
	int	commnad_flag;

	commnad_flag = 0;
	while (token)
	{
		// if (!token->word)
		// 	token->kind = _NULL;
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
		{
			token = add_command_kind(token, commnad_flag);
			add_command_kind(token, commnad_flag);
		}
		if (token->kind == BUILTIN || token->kind == COMMAND)
			commnad_flag++;
		token = token->next;
	}
}

t_token	*lexer(char *line)
{
	t_token	*token;

	if (!(*line))
		return (NULL);
	token = NULL;
	token = tokenizer(line);
	set_token(SET, token);
	add_token_kind(token);
	if (!find_syntax_error(token))
		return (free_token(token), end_status(SET, 1), NULL);
	while (token)
	{
		if (ft_strchr(token->word, '$'))
			token->word = expand_dollar(token->word, token);
		token = token->next;
	}
	token = set_token(GET, NULL);
	while (token)
	{
		if (ft_strchr(token->word, '\'') || ft_strchr(token->word, '\"'))
			token->word = expand_quote(token->word, token);
		token = token->next;
	}
	return (set_token(GET, NULL));
}
