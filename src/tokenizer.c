#include "../minishell.h"

// スペースとタブをスキップする関数
char	*space_skip(char *input)
{
	while (*input == ' ' || *input == '\t' || *input == '\n')
	{
		if (*(input + 1))
			input++;
		else
			break ;
	}
	return (input);
}

// 特殊トークンを作成する関数
t_token	*create_special_token(char **input, t_kind kind, int length)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->word = ft_substr(*input - length, 0, length);
	new_token->kind = kind;
	new_token->next = NULL;
	new_token->pre = NULL;
	input += length;  // トークン長だけポインタを進める
	return (new_token);
}

// 通常の文字列をトークンとして作成する関数
//t_token	*create_command_token(char *start)
t_token	*create_command_token(char *start, bool is_quoted)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->word = start;
	new_token->kind = COMMAND;
	new_token->is_quoted = is_quoted;
	new_token->next = NULL;
	new_token->pre = NULL;
	return (new_token);
}

t_token	*tokenizer(char *input, int *error_status)
{
    t_token *head = NULL;
    t_token *current = NULL;
    t_token *new_token = NULL;
    *error_status = 0;

    input = space_skip(input);
	//new_token = NULL;
	//char *result = NULL;
    while (*input)
	{
        new_token = NULL;

        // 特殊トークン処理
        // ここにシングルクォートを分断する関数を作成
        if (*input == '\'') // シングルクォートが見つかった場合
        {
            new_token = process_single_quote(&input, error_status);
            if (*error_status)
            {
                free_token(head);
                return (NULL);
            }
        }
        else if (*input == '<' || *input == '>' || *input == '|')
		{
            char c = *input;
            int count = 0;

            // 同じ文字が連続する長さをカウント
            while (*input == c)
            {
                count++;
                input++;
            }
            // トークンの種類を決定
            t_kind kind;
            if (c == '<')
                kind = LESSTHAN;
            else if (c == '>')
                kind = MORETHAN;
            else
                kind = PIPE;
            // トークン作成
            new_token = create_special_token(&input, kind, count);
            if (!new_token)
            {
                *error_status = 1;
                free_token(head);
                return (NULL);
            }
        }
		else
		{
            // コマンドやオプションの処理
            char *result = NULL;

			//while (*input && !ft_strchr(SPECIAL_TOKEN, *input) && !ft_isspace(*input))
			while (*input && *input != '\'' && !ft_strchr(SPECIAL_TOKEN, *input) && !ft_isspace(*input))
			{
				result = ft_strjoin_one(result, *input);
				input++;
			}
			if (result)
				new_token = create_command_token(result, false);
            //new_token = create_command_token(result);
			//new_token = create_command_token(result, false);
			if (!new_token)
			{
				*error_status = 1;
				free_token(head);
				return (NULL);
			}
		}

        // トークンリストの接続
		if (!head)
			head = new_token;
		else
		{
			current->next = new_token;
			new_token->pre = current;
		}
		current = new_token;
		input = space_skip(input);
	}

    // トークンの出力デバッグ
    t_token *ptr = head;
    int i = 0;
	while (ptr)
	{
		printf("i = %d, token = %s\n", i++, ptr->word);
		ptr = ptr->next;
	}
	return (head);
}
