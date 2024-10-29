#include "../minishell.h"

char	*make_syntax_str(char c, t_token *token)
{
	char	*str;

	str = NULL;
	str = ft_strdup("bash: syntax error near unexpected token `");
	if (str)
		str = strjoin_1char(str, c);
	if (str)
		str = strjoin_with_free(str, "'\n", FREE_S1);
	if (!str)
		return (NULL);
	token->kind = ERR;
	return (str);
}

static t_token	*add_kind_pipe(t_toke *token)
{
	if (ft_memcmp(token->word, "|", 2))
	{
		token->kind = ERR;
		token->syntax_msg = make_syntax_str('|');
		if (!token->syntax_msg)
			;//token free, line free
		return (token);
	}
	else
		token->kind = PIPE;
	if (token->next)
		return (token->next);
}

static t_token	*add_kind_lessthan(t_toke *token)
{
	if (ft_memcmp(token->word, "<", 2) == 0)
	{
		if (ptr->next && ft_strchr(SPECIAL_TOKEN, *(ptr->next->word)))
		{
			ptr->kind = SKIP;
			ptr->next->kind = RDFILE;
		}
		else
			token->syntax_msg = make_syntax_str('<', token);
	}
	else if (ft_memcmp(token->word, "<<", 3) == 0)
	{
		if (ptr->next && ft_strchr(SPECIAL_TOKEN, *(ptr->next->word)))
		{
			ptr->kind = SKIP;
			ptr->next->kind = LIMITTER;
		}
		else
			token->syntax_msg = make_syntax_str('<', token);
	}
	else
		token->syntax_msg = make_syntax_str('<', token);
	return (token);
}

void	add_token_kind(t_token *token)
{
	t_token	*ptr;

	ptr = token;
	while (ptr)
	{
		if (ptr->kind == ERR)
			break; //syntax errなのでこれ以降は作業しなくて良い
		if (*(ptr->word) == '|')
			ptr = add_kind_pipe(ptr);
		else if (*(ptr->word) == '<')
			add_kind_lessthan(ptr);
		else if (*(ptr->word) == '>')
			add_kind_morethan(ptr);
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
