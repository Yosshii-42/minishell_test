/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_rm_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:59:51 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/22 16:24:05 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// クォートを削除する関数(不要かも。いらなければ削除予定。)
bool remove_quotes(t_token *tokenized)
{
    while (tokenized)
    {
        char *new_word = ft_calloc(ft_strlen(tokenized->word) + 1, sizeof(char));
        int i = 0, j = 0;
        bool in_single_quote = false;
        bool in_double_quote = false;

        if (!new_word)
            return (false);

        while (tokenized->word[i])
        {
            if (tokenized->word[i] == '\'' && !in_double_quote) // シングルクォート
            {
                in_single_quote = !in_single_quote; // クォートの開閉を切り替え
                i++;
            }
            else if (tokenized->word[i] == '"' && !in_single_quote) // ダブルクォート
            {
                in_double_quote = !in_double_quote; // クォートの開閉を切り替え
                i++;
            }
            else
            {
                new_word[j++] = tokenized->word[i++];
            }
        }

        new_word[j] = '\0';
        free(tokenized->word);
        tokenized->word = new_word;

        tokenized = tokenized->next;
    }
    return (true);
}
