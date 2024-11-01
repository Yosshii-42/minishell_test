#include "../minishell.h"

char	*space_skip(char *prompt)
{
	while (*prompt)
	{
		if (*prompt == ' ' || *prompt == '\t')
			prompt++;
		else
			return (prompt);
	}
	return (prompt);
}

t_token	*tokenizer(char	*prompt, int *error_status)
{
	t_token	root_token;
	t_token	*token;

	root_token.word = NULL;
	root_token.kind = SKIP;
	root_token.end = SKIP;
	root_token.next = NULL;
	root_token.pre = NULL;
	token = &root_token;

	while (*prompt)
	{
		prompt = space_skip(prompt);
		if (*prompt == '\0')
			break ;
		token = make_token(&prompt, token, error_status);
		if (*error_status == 1)
			return (root_token.next);
	}
	token->next = make_eof_token();
	return (root_token.next);
}
