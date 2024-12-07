/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_lexer_add_kind.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:49:58 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/07 19:30:10 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*add_kind_pipe(t_token *token)
{
	if (token->is_quoted == true)
		return (token->kind = COMMAND, token);
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

t_token	*add_kind_lessthan(t_token *token)
{
	if (token->is_quoted == true)
		return (token->kind = COMMAND, token);
	if (!token->next)
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

t_token	*add_kind_morethan(t_token *token)
{
	if (token->is_quoted == true)
		return (token->kind = COMMAND, token);
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

t_token	*add_command_kind(t_token *token, int command_flag)
{
	if (token->next && token->next->kind == SYNTAX)
		token->kind = SYNTAX;
	else if (token->pre)
	{
		if (token->pre->kind == LESSTHAN)
			token->kind = RDFILE;
		else if (token->pre->kind == HERE_DOC)
			token->kind = LIMITTER;
		else if (token->pre->kind == MORETHAN)
			token->kind = WRFILE;
		else if (token->pre->kind == APPEND)
			token->kind = WRF_APP;
		else if (command_flag > 0)
			token->kind = OPTION;
		else
			set_kind_as_command(token, &command_flag);
	}
	else
		set_kind_as_command(token, &command_flag);
	if (!token->next)
		token->status = END;
	return (token);
}

int	check_builtin(char *str)
{
	static char	*builtin[] = \
		{"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int			i;

	i = 0;
	while (i < 7)
	{
		if (ft_memcmp(str, builtin[i], ft_strlen(builtin[i]) + 1) == 0)
			return (i);
		i++;
	}
	return (-1);
}
