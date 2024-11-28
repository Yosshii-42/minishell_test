/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:35:00 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/25 00:27:58 by tsururukako      ###   ########.fr       */
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

t_token	*lexer(char *line)
{
	t_token	*token;

	if (!(*line))
		return (NULL);
	token = NULL;
	token = tokenizer(line);
	if (!token)
		return (NULL);
	if (!find_syntax_error(token))
	{
		free_token(token);
		end_status(SET, 1);
		return (NULL);
	}		
	add_token_kind(token);
	return (token);	
}
