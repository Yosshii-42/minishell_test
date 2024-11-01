#include "../minishell.h"

void	init_operators(char **operators)
{
	operators[0] = "||";
	operators[1] = "&&";
	operators[2] = "&";
	operators[3] = ";;";
	operators[4] = ";";
	operators[5] = "(";
	operators[6] = ")";
	operators[7] = "|";
	operators[8] = "\n";
	operators[9] = "<<";
	operators[10] = ">>";
	operators[11] = "<";
	operators[12] = ">";
	operators[13] = "~";
	operators[14] = "`";
	operators[15] = "#";
	operators[16] = "*";
	operators[17] = "[";
	operators[18] = "]";
	operators[19] = "{";
	operators[20] = "}";
	operators[21] = "!";
	operators[22] = "?";
	operators[23] = "\t";
}

t_token	*create_token(char *word, t_kind kind)
{
	t_token	*new_token;
	char	*op_str;

	op_str = malloc_op(prompt);
	if (!op_str)
		return (NULL);
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
	{
		free(op_str);
		return (NULL);
	}
	new_token->word = op_str;
	new_token->kind = PIPE;
	new_token->end = END;
	new_token->next = NULL;
	new_token->pre = NULL;
	return (new_token);
}
