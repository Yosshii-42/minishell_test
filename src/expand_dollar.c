/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 23:37:41 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/27 21:44:57 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
volatile sig_atomic_t g_exit_status = 0;

// 環境変数名を抽出する
static	char	*split_keyname(char *token, int i)
{
	int	len;

	len = count_envname_len(token, i);
	return (ft_substr(token, i, len)); // 環境変数名を切り出し
}

// クォート状態を更新する
static	void	update_quote_status(char *quote, char c)
{
	if (*quote == 0) // 現在クォート外の場合
		*quote = c;  // クォートを開く
	else if (*quote == c) // 現在のクォートと一致する場合
		*quote = 0;  // クォートを閉じる
}

// `$` を処理する
bool	handle_dollar(t_token *tokenized, int *i)
{
    char    *env_key;
    char    *tmp;
    char    *exit_status;
    t_env   *env;

    (*i)++; // `$` をスキップ
    env_key = split_keyname(tokenized->word, *i);
    // if (!env_key)   <--safe_mallocでガード
    // {
    //     return (false);
    // }

	tmp = tokenized->word;

    // $?処理 (これがないとecho $?をしたときに何も表示されない)
    if (ft_strncmp(env_key, "?", 1) == 0)
    {
        exit_status = ft_itoa(end_status(GET, 0)); // <--safe_mallocでガード
        if (tokenized->word[++(*i)])
            exit_status = strjoin_with_free(exit_status, &tokenized->word[*i], FREE_S1);
        // if (!exit_status)   <--safe_mallocでガード
        // {
        //     free(env_key);
        //     return (false);
        // }
        tokenized->word = exit_status;
        // tokenized->kind = OPTION; //これはいらないかも。COMMANDとして認識されるが要件外かも
        // return (free(env_key), free(tmp), true);
        return (free(env_key), true);
    }
    env = set_env(GET, NULL);
    while (env)
    {
        if (ft_memcmp(env_key, env->key, ft_strlen(env_key) + 1) == 0)
        {
    		tokenized->word = ft_strdup(env->value);
            // return (free(tmp), true);
            return (true);
        }
        env = env->next;
    }
    tokenized->word = ft_strdup("");
    // return (free(tmp), true);
    return (true);
}

// トークン内の `$` を展開する
bool	expand_dollar(t_token *tokenized)
{
    int i = 0;
    char quote = 0;
    char *new = ft_strdup("");

    // if (!new)
    //     return (false);

    while (tokenized->word[i])
    {
        if (is_quote(tokenized->word[i]))
            update_quote_status(&quote, tokenized->word[i]);

        if (quote != '\'' && tokenized->word[i] == '$')
        {
            if (!handle_dollar(tokenized, &i))
            {
                free(new);
                return (false);
            }
        }
        else
        {
            if (!append_char(&new, tokenized->word[i]))
            {
                free(new);
                return (false);
            }
            i++;
        }
    }
    //free(tokenized->word);
    //tokenized->word = new;
    //while (tokenized)
    //{
    //    printf("Word: %s, Kind: %d, Quoted: %d, Double Quoted: %d\n",
    //           tokenized->word, tokenized->kind, tokenized->is_quoted, tokenized->is_double_quoted);
    //    tokenized = tokenized->next;
    //}
    free(new);
    return (true);
}

// #include "../minishell.h"
// volatile sig_atomic_t g_exit_status = 0;

// // 環境変数名を抽出する
// static	char	*split_keyname(char *token, int i)
// {
// 	int	len;

// 	len = count_envname_len(token, i);
// 	return (ft_substr(token, i, len)); // 環境変数名を切り出し
// }

// // クォート状態を更新する
// static	void	update_quote_status(char *quote, char c)
// {
// 	if (*quote == 0) // 現在クォート外の場合
// 		*quote = c;  // クォートを開く
// 	else if (*quote == c) // 現在のクォートと一致する場合
// 		*quote = 0;  // クォートを閉じる
// }

// // `$` を処理する
// bool	handle_dollar(t_token *tokenized, int *i)
// {
//     char    *env_key;
//     char    *tmp;
//     char    *exit_status;
//     t_env   *env;

//     (*i)++; // `$` をスキップ
//     env_key = split_keyname(tokenized->word, *i);
//     // if (!env_key)
//     // {
//     //     return (false);
//     // }

// 	tmp = tokenized->word;

//     // $?処理 (これがないとecho $?をしたときに何も表示されない)
//     if (ft_strncmp(env_key, "?", 1) == 0)
//     {
//         exit_status = ft_itoa(end_status(GET, 0));
//         // if (tokenized->word[++(*i)])
//         //     exit_status = strjoin_with_free(exit_status, &tokenized->word[*i], FREE_S1);
//         // if (!exit_status)
//         // {
//         //     free(env_key);
//         //     return (false);
//         // }
//         tokenized->word = exit_status;
//         // tokenized->kind = OPTION; //これはいらないかも。COMMANDとして認識されるが要件外かも
//         return (free(env_key), free(tmp), true);
//     }
//     env = set_env(GET, NULL);
//     while (env)
//     {
//         if (ft_memcmp(env_key, env->key, ft_strlen(env_key) + 1) == 0)
//         // {
//     		tokenized->word = ft_strdup(env->value);
//             // return (free(tmp), true);
//         // }
//         env = env->next;
//     }
//     tokenized->word = ft_strdup("");
//     return (free(tmp), true);
// }

// // トークン内の `$` を展開する
// bool	expand_dollar(t_token *tokenized)
// {
//     int i = 0;
//     char quote = 0;
//     char *new;
    
//     new = ft_strdup("");
//     while (tokenized->word[i])
//     {
//         if (is_quote(tokenized->word[i]))
//             update_quote_status(&quote, tokenized->word[i]);
//         if (quote != '\'' && tokenized->word[i] == '$')
//             handle_dollar(tokenized, &i);
//         // {
//         //     if (!handle_dollar(tokenized, &i))
//         //         return (free(new), false);
//         // }
//         else
//         {
//             append_char(&new, tokenized->word[i]);
//         // {
//         //     if (!append_char(&new, tokenized->word[i]))
//         //         return (free(new), false);
//             i++;
//         }
//     }
//     //free(tokenized->word);
//     //tokenized->word = new;
//     //while (tokenized)
//     //{
//     //    printf("Word: %s, Kind: %d, Quoted: %d, Double Quoted: %d\n",
//     //           tokenized->word, tokenized->kind, tokenized->is_quoted, tokenized->is_double_quoted);
//     //    tokenized = tokenized->next;
//     //}

//     return (true);
// }
