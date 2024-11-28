/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_dollar.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 23:37:41 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/28 18:55:54 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

static	char	*split_keyname(char *token, int i)
{
	int	len;

	len = count_envname_len(token, i);
	return (ft_substr(token, i, len));
}

static	void	update_quote_status(char *quote, char c)
{
	if (*quote == 0)
		*quote = c;
	else if (*quote == c)
		*quote = 0;
}

static	char	*resolve_env_value(char	*env_key)
{
	t_env	*env;

	env = set_env(GET, NULL);
	while (env)
	{
		if (ft_memcmp(env_key, env->key, ft_strlen(env_key) + 1) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

bool	handle_dollar(t_token *tokenized, int *i)
{
	char	*env_key;
	char	*tmp;

	(*i)++;
	env_key = split_keyname(tokenized->word, *i);
	tmp = tokenized->word;
	if (ft_strncmp(env_key, "?", 1) == 0)
	{
		tokenized->word = strjoin_with_free(ft_itoa(end_status(GET, 0)), \
			&tokenized->word[++(*i)], FREE_S1);
		return (free(env_key), true);
	}
	tokenized->word = resolve_env_value(env_key);
	return (free(tmp), free(env_key), true);
}

bool	expand_dollar(t_token *tokenized)
{
	int		i;
	char	quote;
	char	*new;

	i = 0;
	quote = 0;
	new = ft_strdup("");
	while (tokenized->word[i])
	{
		if (is_quote(tokenized->word[i]))
			update_quote_status(&quote, tokenized->word[i]);
		if (quote != '\'' && tokenized->word[i] == '$')
			handle_dollar(tokenized, &i);
		else
		{
			append_char(&new, tokenized->word[i]);
			i++;
		}
	}
	free(new);
	return (true);
}
