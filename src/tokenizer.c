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
////t_token	*create_command_token(char *start)
//t_token	*create_command_token(char *start, bool is_quoted)
//{
//	t_token	*new_token;

//	new_token = (t_token *)malloc(sizeof(t_token));
//	if (!new_token)
//		return (NULL);
//	new_token->word = start;
//	new_token->kind = COMMAND;
//	new_token->is_quoted = is_quoted;
//	new_token->next = NULL;
//	new_token->pre = NULL;
//	return (new_token);
//}

t_token	*create_command_token(char *start, bool is_quoted, bool is_double_quoted)
{
    t_token *new_token;

    new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return (NULL);
    new_token->word = start;
    new_token->kind = COMMAND;
    new_token->is_quoted = is_quoted;
    new_token->is_double_quoted = is_double_quoted;
    new_token->next = NULL;
    new_token->pre = NULL;
    return (new_token);
}

void append_token(t_token **head, t_token **current, t_token *new_token)
{
    if (!*head)
        *head = new_token;
    else
    {
        (*current)->next = new_token;
        new_token->pre = *current;
    }
    *current = new_token;
}

t_token *process_quote(char **input, int *error_status, char quote_char)
{
    t_token *new_token;
    char *quoted_content = NULL;
    bool is_double_quoted = (quote_char == '"');

    (*input)++;  // クォートの開始をスキップ
    while (**input && **input != quote_char)
    {
        quoted_content = ft_strjoin_one(quoted_content, **input);
        (*input)++;
    }
    if (**input == '\0')
    {
        fprintf(stderr, "error: unmatched quote\n");
        free(quoted_content);
        *error_status = 1;
        return (NULL);
    }
    (*input)++;  // クォートの閉じをスキップ
    new_token = create_command_token(quoted_content, !is_double_quoted, is_double_quoted);
    return (new_token);
}

t_token *tokenizer(char *input, int *error_status)
{
    t_token *head = NULL;
    t_token *current = NULL;
    *error_status = 0;

    input = space_skip(input);

    while (*input)
    {
        t_token *new_token = NULL;

        // 1. 特殊トークン処理
        if (ft_strchr(SPECIAL_TOKEN, *input))
        {
            char c = *input;
            int count = 0;

            // 同じ特殊文字が連続する長さをカウント
            while (*input == c)
            {
                count++;
                input++;
            }
            t_kind kind;
            if (c == '<')
                kind = LESSTHAN;
            else if (c == '>')
                kind = MORETHAN;
            else
                kind = PIPE;

            new_token = create_special_token(&input, kind, count);
            if (!new_token)
            {
                *error_status = 1;
                free_token(head);
                return NULL;
            }
        }
        // 2. シングルクォート処理
        else if (*input == '\'')
        {
            new_token = process_quote(&input, error_status, '\'');
            if (*error_status)
            {
                free_token(head);
                return NULL;
            }
        }
        // 3. ダブルクォート処理
        else if (*input == '"')
        {
            new_token = process_quote(&input, error_status, '"');
            if (*error_status)
            {
                free_token(head);
                return NULL;
            }
        }
        else
        {
            // 通常の文字列の処理
            char *result = NULL;

            while (*input && *input != '\'' && *input != '"' &&
                   !ft_strchr(SPECIAL_TOKEN, *input) && !ft_isspace(*input))
            {
                result = ft_strjoin_one(result, *input);
                input++;
            }
            new_token = create_command_token(result, false, false);
            if (!new_token)
            {
                *error_status = 1;
                free_token(head);
                return NULL;
            }
        }

        // トークンリストの接続
        append_token(&head, &current, new_token);
        input = space_skip(input);
    }

    return head;
}

//t_token	*tokenizer(char *input, int *error_status)
//{
//    t_token *head = NULL;
//    t_token *current = NULL;
//    t_token *new_token = NULL;
//    *error_status = 0;

//    input = space_skip(input);
//	//new_token = NULL;
//	//char *result = NULL;
//    while (*input)
//	{
//        new_token = NULL;

//        // 特殊トークン処理
//        // ここにシングルクォートを分断する関数を作成
//        if (*input == '\'') // シングルクォートが見つかった場合
//        {
//            new_token = process_single_quote(&input, error_status);
//            if (*error_status)
//            {
//                free_token(head);
//                return (NULL);
//            }
//        }
//        else if (*input == '<' || *input == '>' || *input == '|')
//		{
//            char c = *input;
//            int count = 0;

//            // 同じ文字が連続する長さをカウント
//            while (*input == c)
//            {
//                count++;
//                input++;
//            }
//            // トークンの種類を決定
//            t_kind kind;
//            if (c == '<')
//                kind = LESSTHAN;
//            else if (c == '>')
//                kind = MORETHAN;
//            else
//                kind = PIPE;
//            // トークン作成
//            new_token = create_special_token(&input, kind, count);
//            if (!new_token)
//            {
//                *error_status = 1;
//                free_token(head);
//                return (NULL);
//            }
//        }
//		else
//		{
//            // コマンドやオプションの処理
//            char *result = NULL;

//			//while (*input && !ft_strchr(SPECIAL_TOKEN, *input) && !ft_isspace(*input))
//			while (*input && *input != '\'' && !ft_strchr(SPECIAL_TOKEN, *input) && !ft_isspace(*input))
//			{
//				result = ft_strjoin_one(result, *input);
//				input++;
//			}
//			if (result)
//				new_token = create_command_token(result, false);
//            //new_token = create_command_token(result);
//			//new_token = create_command_token(result, false);
//			if (!new_token)
//			{
//				*error_status = 1;
//				free_token(head);
//				return (NULL);
//			}
//		}

//        // トークンリストの接続
//		if (!head)
//			head = new_token;
//		else
//		{
//			current->next = new_token;
//			new_token->pre = current;
//		}
//		current = new_token;
//		input = space_skip(input);
//	}

//    // トークンの出力デバッグ
//    t_token *ptr = head;
//    int i = 0;
//	while (ptr)
//	{
//		printf("i = %d, token = %s\n", i++, ptr->word);
//		ptr = ptr->next;
//	}
//	return (head);
//}
