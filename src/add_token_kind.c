#include "../minishell.h"

// char	*make_syntax_str(char c, t_token *token)
// {
// 	char	*str;

// 	str = NULL;
// 	str = ft_strdup("bash: syntax error near unexpected token `");
// 	if (str)
// 		str = strjoin_1char(str, c);
// 	if (str)
// 		str = strjoin_with_free(str, "'\n", FREE_S1);
// 	if (!str)
// 		return (NULL);
// 	token->kind = ERR;
// 	return (str);
// }

// static t_token	*add_kind_pipe(t_toke *token)
// {
// 	if (ft_memcmp(token->word, "|", 2)								// |が２つ以上続く
// 		|| (token->pre && ft_strchr(TOKEN_1, *(token->pre->word)))	// 前のtokenが"<>|"のどれか
// 			|| !(token->pre))										// 前のtokenが無い
// 	{
// 		token->kind = ERR;
// 		token->syntax_msg = make_syntax_str('|');
// 		if (!token->syntax_msg)										// mallocエラーならNULLを返す
// 			return (NULL);
// 		// return (token);
// 	}
// 	else
// 	{
// 		token->kind = PIPE;
// 		if (token->next)											// 次のtokenに進める
// 			token = token->next;
// 		else
// 			token->status = END;									//　次のtokenがない時はstatusをENDにする
// 	}
// 	return (token);
// }

// static t_token	*add_kind_lessthan(t_toke *token)
// {
// 	if (token->pre && ft_strchr(TOKEN_1, *(token->pre->word)))
// 	{
// 		if (*(token->pre->word) == '<' || *(token->pre->wore) == '>')
// 			token->syntax_msg = make_syntax_str('<');
// 		else
// 			token->syntax_msg = make_syntax_str('|');
// 		token->kind = ERR;
// 		if (!token->syntax_msg)										// mallocエラーならNULLを返す
// 			return (NULL);
// 		// return (token);
// 	}
// 	if (!ft_memcmp(token->word, "<", 2) || !ft_memcmp(token->word, "<<", 3))
// 	{
// 		token->kind = SKIP;
// 		if (token->next)
// 			token = token->next;
// 		else
// 			token->status = END;
// 	}
// 	else if (ft_memcmp(token->word, "<<", 3) > 0)
// 	{
// 		token->syntax_msg = make_syntax_str('<');
// 		token->kind = ERR;
// 		if (!token->syntax_msg)										// mallocエラーならNULLを返す
// 			return (NULL);
// 	}
// 	return (token);
// }

// int	add_token_kind(t_token *token)
// {
// 	t_token	*ptr;

// 	ptr = token;
// 	while (ptr)
// 	{
// 		if (ptr->status == FREE)
// 			return (FALSE);
// 		if (ptr->kind == ERR || ptr->status == END)
// 			return (TRUE); //syntax err 又はtokenの最後なのでこれ以降は作業しなくて良い
// 		if (*(ptr->word) == '|' && !(ptr = add_kind_pipe(ptr)))
// 			return (FALSE);
// 		else if (*(ptr->word) == '<' && !(ptr = add_kind_lessthan(ptr)))
// 			return (FALSE);
// 		else if (*(ptr->word) == '>' && !(ptr = add_kind_morethan(ptr)))
// 			return (FALSE);
// 		else
// 		{
// 			if (!ptr->pre || ptr->pre->kind != COMMAND)
// 				ptr->kind = COMMAND;
// 			else if (ptr->pre->kind == COMMAND)
// 				ptr->kind = OPTION;
// 			if (ptr->next)
// 				ptr = ptr->next;
// 			else
// 				return (TRUE);
// 		}
// 	}
// }

void	add_token_kind(t_token *token)
{
	t_token	*ptr;

	ptr = token;
	while (ptr)
	{
		if (!ft_memcmp(ptr->word, "|", 2))
		{
			ptr->kind = PIPE;
			if (ptr->next)
				ptr = ptr->next;
		}
		else if (!ft_memcmp(ptr->word, "<", 2))
		{
			ptr->kind = SKIP;
			ptr->next->kind = RDFILE;
			ptr = ptr->next->next;
		}
		else if (!ft_memcmp(ptr->word, "<<", 3))
		{
			ptr->kind = SKIP;
			ptr->next->kind = LIMITTER;
			ptr = ptr->next->next;
		}
		else if (!ft_memcmp(ptr->word, ">", 2))
		{
			ptr->kind = SKIP;
			ptr->next->kind = WRFILE;
			ptr = ptr->next->next;
		}
		else if (!ft_memcmp(ptr->word, ">>", 3))
		{
			ptr->kind = SKIP;
			ptr->next->kind = WRFILE_APP;
			ptr = ptr->next->next;
		}
		else
		{
			if (!ptr->pre || ptr->pre->kind != COMMAND)
				ptr->kind = COMMAND;
			else if (ptr->pre->kind == COMMAND)
				ptr->kind = OPTION;
			if (ptr->next)
				ptr = ptr->next;
			else
				break;
		}
	}
}
