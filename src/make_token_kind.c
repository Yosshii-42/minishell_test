/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_token_kind.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:28:06 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/05 22:00:48 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	token_count(t_token *token)
{
	int	count;

	count = 0;
	if (!token)
		return (0);
	while (token)
	{
		count++;
		if (token->next)
			token = token->next;
		else
			break ;
	}
	return (count);
}

static t_token	*add_kind_pipe(t_token *token)
{
	if (!token->pre && !token->next)
		return (token->kind = SYNTAX, token);
	if (!token->pre || ft_strlen(token->word) > 1)
		return (token->kind = SYNTAX, token);
	else if (token->pre && ft_strchr(SPECIAL_TOKEN, *(token->pre->word)))
		token->kind = SYNTAX;
	else
	{
		token->kind = PIPE;
		if (!token->next)
			token->status = END;
	}
	return (token);
}

static t_token	*add_kind_lessthan(t_token *token)
{
	if (!token->pre && !token->next)
		return (token->kind = SYNTAX, token);
	if (ft_strlen(token->word) > 2)
		return (token->kind = SYNTAX, token);
	else if (token->pre && (*(token->pre->word) == '<'
			|| *(token->pre->word) == '>'))
		token->kind = SYNTAX;
	else
	{
		if (ft_strlen(token->word) == 1)
			token->kind = LESSTHAN;
		else
			token->kind = HERE_DOC;
		if (!token->next)
			token->status = END;
	}
	return (token);
}

static t_token	*add_kind_morethan(t_token *token)
{
	if (!token->next)
		return (token->kind = SYNTAX, token);
	if (ft_strlen(token->word) > 2)
		return (token->kind = SYNTAX, token);
	if (token->pre && (*(token->pre->word) == '<'
			|| *(token->pre->word) == '>'))
		token->kind = SYNTAX;
	{
		if (ft_strlen(token->word) == 1)
			token->kind = MORETHAN;
		else
			token->kind = APPEND;
		if (!token->next)
			token->status = END;
	}
	return (token);
}

static t_token	*add_command_kind(t_token *token)
{
	if (token->pre)
	{
		if (token->pre->kind == LESSTHAN)
			token->kind = RDFILE;
		else if (token->pre->kind == HERE_DOC)
			token->kind = LIMITTER;
		else if (token->pre->kind == MORETHAN)
			token->kind = WRFILE;
		else if (token->pre->kind == APPEND)
			token->kind = WRF_APP;
		else if (token->pre->kind == COMMAND)
			token->kind = OPTION;
		else if (token->pre->kind == OPTION)
			token->kind = OPTION;
		else
			token->kind = COMMAND;
	}
	else
		token->kind = COMMAND;
	if (token->kind == COMMAND && !ft_memcmp(token->word, "cat", 4))
		token->status = CAT;
	if (!token->next)
		token->status = END;
	return (token);
}

void	add_token_kind(t_token *token, int status_num)
{
	while (token)
	{
		if (*(token->word) == '|')
			token = add_kind_pipe(token);
		else if (*(token->word) == '<')
			token = add_kind_lessthan(token);
		else if (*(token->word) == '>')
			token = add_kind_morethan(token);
		else if (!ft_memcmp(token->word, "$?", 3))// TODO '' ""実装後に再実装する
		{
			token->kind = OPTION;
			if (!token->next)
				token->status = END;
			token->word = ft_itoa(status_num);
		}
		else
			token = add_command_kind(token);
		if (token->next)
			token = token->next;
		else
			break ;
	}
}
