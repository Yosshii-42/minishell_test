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
			break;
	}
	return (count);
}

static t_token	*add_kind_pipe(t_token *token)
{
	if (!token->pre && !token->next)
		return (token->kind = SYNTAX, token);
	if (!token->pre || ft_strlen(token->word) > 1)				// 前のtokenが無いor |が２つ以上続く
		return (token->kind = SYNTAX, token);
	else if (token->pre && (*(token->pre->word) == '|' || *(token->pre->word) == '<'))
		token->kind = SYNTAX;
	else
	{
		token->kind = PIPE;
		if (!token->next)											// 次のtokenに進める
			token->status = END;								//　次のtokenがない時はstatusをENDにする
	}
	return (token);
}

static t_token	*add_kind_lessthan(t_token *token)
{
	if (!token->pre && !token->next)
		return (token->kind = SYNTAX, token);
	if (ft_strlen(token->word) > 2)
		return (token->kind = SYNTAX, token);
	else if (token->pre &&
		(*(token->pre->word) == '<' || *(token->pre->word) == '>'))
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
	if (!token->pre && !token->next)
		return (token->kind = SYNTAX, token);
	if (ft_strlen(token->word) > 2)
		return (token->kind = SYNTAX, token);
	if (token->pre &&
		(*(token->pre->word) == '<' || *(token->pre->word) == '>'))
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
	if (!token->next)
		token->status = END;
	return (token);
}

void	add_token_kind(t_token *token)
{
	while (token)
	{
		if (*(token->word) == '|')
			token = add_kind_pipe(token);	// 
		else if (*(token->word) == '<')
			token = add_kind_lessthan(token);
		else if (*(token->word) == '>')
			token = add_kind_morethan(token);
		else
			token = add_command_kind(token);
		if (token->next)
			token = token->next;
		else
			break;
	}
}


// void add_token_kind(t_token *token)
// {
// 	t_token	*ptr;

// 	ptr = token;
// 	while (ptr)
// 	{
// 		if (!ft_memcmp(ptr->word, "|", 2))
// 		{
// 			ptr->kind = PIPE;
// 			if (ptr->next)
// 				ptr = ptr->next;
// 		}
// 		else if (!ft_memcmp(ptr->word, "<", 2))
// 		{
// 			ptr->kind = SKIP;
// 			ptr->next->kind = RDFILE;
// 			ptr = ptr->next->next;
// 		}
// 		else if (!ft_memcmp(ptr->word, "<<", 3))
// 		{
// 			ptr->kind = SKIP;
// 			ptr->next->kind = LIMITTER;
// 			ptr = ptr->next->next;
// 		}
// 		else if (!ft_memcmp(ptr->word, ">", 2))
// 		{
// 			ptr->kind = SKIP;
// 			ptr->next->kind = WRFILE;
// 			ptr = ptr->next->next;
// 		}
// 		else if (!ft_memcmp(ptr->word, ">>", 3))
// 		{
// 			ptr->kind = SKIP;
// 			ptr->next->kind = WRFILE_APP;
// 			ptr = ptr->next->next;
// 		}
// 		else
// 		{
// 			if (!ptr->pre || ptr->pre->kind != COMMAND)
// 				ptr->kind = COMMAND;
// 			else if (ptr->pre->kind == COMMAND)
// 				ptr->kind = OPTION;
// 			if (ptr->next)
// 				ptr = ptr->next;
// 			else
// 				break;
// 		}
// 	}
// }
