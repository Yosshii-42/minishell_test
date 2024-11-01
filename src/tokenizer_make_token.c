#include "../minishell.h"

t_token *make_word_token(char **prompt, int *error_status)
{
	int		i;
	char	*wotd_str;
	t_token	*new_token;

	i = 0;
	while (prompt[i] && !is_meta(prompt[i]))
		i++;
	word_str = ft_substr(prompt, 0, i);
	if (!word_str)
	{
		*error_status = 1;
		return (NULL);
	}
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
	{
		free(word_str);
		return (NULL);
	}
	new_token->word = word_str;
	new_token->kind = COMMAND;
	new_token->next = NULL;
	new_token->pre = NULL;

	return (new_token);
}

t_token	*make_eof_token(void)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->word = NULL;
	new_token->kind = END;
	new_token->next = NULL;
	new_token->pre = NULL;
	return (new_token);
}

t_token *make_token(char **prompt, t_token *parent_token, int *error_status)
{
	t_token	*token;
	int		set_size;

	it (is_operand(*prompt))
	{
		token = create_token(ft_substr(*prompt, 0, 1), PIPE);
		(*prompt)++;
	}
	else
	{
		token = make_word_token(*prompt, error_status);
		if (*error_status == 1)
		{
			parent_token->next = make_eof_token();
			while (**prompt)
				(*prompt)++;
			return (NULL);
		}
		set_size = ft_strlen(token->word);
		(*prompt) += set_size;
	}
	parent_token->next = token;
	token->pre = parent_token;
	return (token);
}
