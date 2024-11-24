/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:35:00 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/22 16:24:28 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*lexer(char *line)
{
	t_token	*tokenized;

	if (!(*line))
		return (NULL);
	// 1.tokenizerを呼び出しトークンリスト生成
	printf("lexer\n");
	tokenized = tokenizer(line);
	if (!tokenized)
		return (NULL);
	// 2.シンタックスエラー確認
	if (!find_syntax_error(tokenized))
	{
		free_token(tokenized);
		end_status(SET, 1);
		return (NULL);
	}
	// 3.環境変数の展開
	// if (!expand_token(env, tokenized))
	// {
	// 	free_token(tokenized);
	// 	*error_status = 1;
	// 	return (NULL);
	// }
	// 4. クォート削除(不要かも。いらなければ削除予定。)
	//remove_quotes(tokenized);
	return (tokenized);
}
