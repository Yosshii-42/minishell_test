#include "../minishell.h"

char	*ft_strjoin_one(char *str, char c)
{
	char	*new_str;
	size_t	len;

	len = 0;
	if (str)
		len = ft_strlen(str);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
		return (NULL);
	if (str)
	{
		ft_strcpy(new_str, str);
		free(str);
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*d;

	d = dest;
	while (*src)
		*d++ = *src++;
	*d = '\0';
	return (dest);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || \
	c == '\n' || c == '\r' || c == '\f' || c == '\v');
}


//char *handle_quotes(char *input, t_token *token, int *error_status) {
//    char quote = *input;
//    char *start = ++input;
//    token->is_quoted = true;
//    token->is_double_quoted = (quote == '"');

//    while (*input && *input != quote)
//        input++;
//    if (*input == '\0') {
//        *error_status = 1;
//        return NULL;
//    }
//    token->word = ft_substr(start - 1, 0, input - start + 2);
//    input++;
//    return input;
//}

//// シングルクォート処理
//char *process_single_quote(char **input) {
//    char *start = ++(*input);  // クォートの次から処理開始
//    while (**input && **input != '\'') {
//        (*input)++;
//    }
//    if (**input == '\'') {
//        size_t len = *input - start;
//        (*input)++;
//        return ft_substr(start, 0, len);  // クォート内の内容を返す
//    }
//    return NULL;  // エラー処理としてNULLを返す（クォートが閉じられていない場合）
//}

//// ダブルクォート処理
//char *process_double_quote(char **input, t_env *env) {
//    char *result = ft_strdup("");  // 最終結果を保存
//    (*input)++;  // クォートの次から処理開始

//    while (**input && **input != '\"') {
//        if (**input == '$') {
//            char *var = expand_variable(input, env);  // 環境変数を展開
//            result = strjoin_with_free(result, var, 1);
//        } else {
//            char temp[2] = { **input, '\0' };
//            result = strjoin_with_free(result, temp, 1);
//            (*input)++;
//        }
//    }
//    if (**input == '\"') {
//        (*input)++;
//    }
//    return result;
//}

//char *expand_variable(char **input, t_env *env) {
//    (*input)++;  // $ の次の文字から処理
//    char *start = *input;
//    while (**input && ft_isalnum(**input)) {
//        (*input)++;
//    }
//    size_t len = *input - start;
//    char *var_name = ft_substr(start, 0, len);
//    char *value = getenv_str(env, var_name);
//    free(var_name);
//    return value ? ft_strdup(value) : ft_strdup("");  // 展開された変数を返す
//}
