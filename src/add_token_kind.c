#include "../minishell.h"

// int	token_count(t_token *token)
// {
// 	int	count;

// 	count = 0;
// 	if (!token)
// 		return (0);
// 	while (token)
// 	{
// 		count++;
// 		if (token->next)
// 			token = token->next;
// 		else
// 			break;
// 	}
// 	return (count);
// }

// char	*make_syntax_str(char c, t_token *token)
// {
// 	char	*str;

// 	str = NULL;
// 	str = ft_strdup("bash: syntax error\n");
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
// 		token->kind = SYNTAX;
// 		token->syntax_msg = make_syntax_str();
// 		if (!token->syntax_msg)										// mallocエラー
// 			token->status = ERR;
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
// 	if (token->pre && ft_strchr(TOKEN_1, *(token->pre->word)) ||
// 		ft_strlen(token->word) > 2)
// 	{
// 		token->kind = SYNTAX;
// 		token->syntax_msg = make_syntax_str();
// 		if (!token->syntax_msg)										// mallocエラー
// 			token->status = ERR;
// 	}
// 	if (!ft_memcmp(token->word, "<", 2) || !ft_memcmp(token->word, "<<", 3))
// 	{
// 		token->kind = LESSTHAN;
// 		if (token->next)
// 			token = token->next;
// 		else
// 			token->status = END;
// 	}
// 	return (token);
// }

// static t_token	*add_kind_morethan(t_toke *token)
// {
// 	if (token->pre && ft_strchr(TOKEN_1, *(token->pre->word)) ||
// 		ft_strlen(token->word) > 2)
// 	{
// 		token->kind = SYNTAX;
// 		token->syntax_msg = make_syntax_str();
// 		if (!token->syntax_msg)										// mallocエラー
// 			token->status = ERR;
// 	}
// 	if (!ft_memcmp(token->word, ">", 2) || !ft_memcmp(token->word, ">>", 3))
// 	{
// 		token->kind = MORETHAN;
// 		if (token->next)
// 			token = token->next;
// 		else
// 			token->status = END;
// 	}
// 	return (token);
// }

// static t_token	*add_command_kind(t_token *token)
// {

// }

// int	add_token_kind(t_token *token)
// {
// 	while (token)
// 	{
// 		if (ptr->status == ERR)
// 			return (FALSE);
// 		if (ptr->kind == SYNTAX || ptr->status == END)
// 			return (TRUE); //syntax err 又はtokenの最後なのでこれ以降は作業しなくて良い
// 		if (*(ptr->word) == '|')
// 			token = add_kind_pipe(token);	// 
// 		else if (*(ptr->word) == '<')
// 			token = add_kind_lessthan(token);
// 		else if (*(ptr->word) == '>')
// 			token = add_kind_morethan(token);
// 		else
// 			token = add_command_kind(token);

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


void add_token_kind(t_token *token)
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
