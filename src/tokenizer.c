/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:28:45 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/29 18:29:49 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	count_quote_len(char *line, char quote_char, bool *is_quoted, bool *is_double_quoted)
{
	int	len;

	len = 1;
	*is_quoted = (quote_char == '\'');
	*is_double_quoted = (quote_char == '"');

	while (line[len] && line[len] != quote_char)
		len++;
	if (line[len] == quote_char)
		return (len + 1);
	return (-1);
}

static int	count_word_len(char *line, bool *is_quoted, bool *is_double_quoted)
{
	int	count;

	count = 0;
	while (line[count] && !ft_isspace(line[count]) && \
		!ft_strchr(SPECIAL_TOKEN, line[count]))
	{
		if (line[count] == '\'' || line[count] == '"')
		{
			int quote_len = count_quote_len(&line[count], line[count], is_quoted, is_double_quoted);
			if (quote_len == -1)
				return (-1);
			count += quote_len;
		}
		else if (line[count] == '$')
		{
			int env_len = count_envname_len(&line[count + 1], 0); // `$` の次を渡す
            if (env_len > 0)
                count += env_len + 1; // `$` と環境変数名の長さを加算
            else
                count++;
        }
        else
            count++;
    }
    return (count);
}

// トークンを作成してリストに追加
static t_token *append_token(t_token **head, t_token **current, char *content, t_kind kind, bool is_quoted, bool is_double_quoted)
{
	t_token	*new_token;

	new_token = (t_token *)safe_malloc(1, sizeof(t_token));
	new_token->word = content;
	new_token->kind = kind;
	new_token->is_quoted = is_quoted;
	new_token->is_double_quoted = is_double_quoted;
	new_token->next = NULL;
	new_token->pre = NULL;

	if (*current)
	{
		(*current)->next = new_token;
		new_token->pre = *current;
	}
	else
		*head = new_token;
	*current = new_token;
	return (new_token);
}

static t_token *create_dollar_token(char **input)
{
    char *env_key;
    int len;

    (*input)++; // `$` をスキップ

    // 環境変数名の長さを計算
    len = count_envname_len(*input, 0);
    if (len == 0)
    {
        end_status(SET, EXIT_FAILURE);
        ft_printf(2, "[DEBUG] Error: Invalid environment variable name after `$`\n");
        return (NULL);
    }
	(*input)--;
    // 環境変数名を抽出
    env_key = ft_substr(*input, 0, len + 1);
    // if (!env_key)
    // {
    //     end_status(SET, EXIT_FAILURE);
    //     perror("[DEBUG] Error: malloc failed while extracting env_key");
    //     return (NULL);
    // }

    // 入力ポインタを環境変数名の長さ分進める
    *input += len + 1;

    // 新しいトークンを作成
    t_token *dollar_token;

	dollar_token = (t_token *)safe_malloc(1, sizeof(t_token));
	dollar_token->word = env_key;
	dollar_token->kind = OPTION;
	dollar_token->is_quoted = false;
	dollar_token->is_double_quoted = false;
	dollar_token->next = NULL;
	dollar_token->pre = NULL;
	return (dollar_token);
}

// トークナイズ処理
t_token *tokenizer(char *input)
{
	t_token	*head;
	t_token	*current;
	char	*token_content;
	int		token_len;
	bool	is_quoted;
	bool	is_double_quoted;

	head = NULL;
	current = NULL;
	is_quoted = false;
	is_double_quoted = false;
	input = space_skip(input);
	while (*input)
	{
		if (ft_strchr(SPECIAL_TOKEN, *input))
		{
			token_len = count_meta_len(input);
			token_content = ft_substr(input, 0, token_len);
			append_token(&head, &current, token_content, (*input == '<') ? LESSTHAN : (*input == '>') ? MORETHAN : PIPE, is_quoted, is_double_quoted);
			input += token_len;
		}
		else if (*input == '$')
		{
			t_token *dollar_token = create_dollar_token(&input);
			if (!dollar_token)
			{
				free_token(head);
				end_status(SET, EXIT_FAILURE);
				return (NULL);
			}
			append_token(&head, &current, dollar_token->word, COMMAND, is_quoted, is_double_quoted);
			free(dollar_token);
		}
		else
		{
			token_len = count_word_len(input, &is_quoted, &is_double_quoted);
			if (token_len == -1)
				return (ft_printf(2, "error: unmatched quote\n"), \
					free_token(head), end_status(SET, EXIT_FAILURE), NULL);

			token_content = ft_substr(input, 0, token_len);
            append_token(&head, &current, token_content, COMMAND, is_quoted, is_double_quoted);
			input += token_len;
		}
		input = space_skip(input);
	}
	return (head);
}
