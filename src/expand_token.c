/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:44:28 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/24 22:34:50 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// 文字を追加する関数
bool	append_char(char **str, char c)
{
	char	*new;
	char	join[2];

	join[0] = c;
	join[1] = '\0';
	new = ft_strjoin(*str, join);
	if (!new)
		return (false);
	free(*str);
	*str = new;
	return (true);
}

// トークンを展開する
bool	expand_token(t_env *env, t_token *tokenized)
{
	// int	flag;

	// flag = 0;
	while (tokenized)
	{
		// コマンドや文字列トークンだけが対象
		if (tokenized->kind == COMMAND || tokenized->kind == OPTION)
		{
			// 環境変数の展開
			if (!expand_dollar(env, tokenized))
				return (false);
			// クォートの処理
			if (!expand_quote(tokenized))
				return (false);
		}
	tokenized = tokenized->next;
	}
	return (true);
}
