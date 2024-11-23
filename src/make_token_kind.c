/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_token_kind.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:28:06 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/09 17:52:05 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static t_token	*add_command_kind(t_token *token, int command_flag)
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
		else if (command_flag > 0)
			token->kind = OPTION;
		else
		{
			token->kind = COMMAND;
			command_flag++;
		}
	}
	else
	{
		token->kind = COMMAND;
		command_flag++;
	}
	if (!token->next)
		token->status = END;
	return (token);
}

int	check_builtin(char *str)
{
	static char	*builtin[] =
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

void	add_token_kind(t_token *token)
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
		// else if (!ft_memcmp(token->word, "$?", 3))// TODO '' ""実装後に再実装する
		// {
		// 	token->kind = OPTION;
		// 	if (!token->next)
		// 		token->status = END;
		// 	token->word = ft_itoa(status_num);
		// }
		else if (check_builtin(token->word) >= 0)
			token->kind = BUILTIN;
		else
			token = add_command_kind(token, commnad_flag);
		if (token->kind == BUILTIN || token->kind == COMMAND)
			commnad_flag++;
		token = token->next;
		// if (token->next)
		// 	token = token->next;
		// else
		// 	break ;
	}
}
