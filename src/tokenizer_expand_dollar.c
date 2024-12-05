/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_dollar.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 23:37:41 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/05 04:04:17 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	dollar_len(char *token_word)
{
	int	len;

	len = 0;
	while (token_word[len])
	{
		if (!(token_word[len] == '\'' || token_word[len] == '\"'
				|| token_word[len] == '$'))
			len++;
		else if (token_word[len] == '\'' || token_word[len] == '\"'
			|| token_word[len] == '$')
			break ;
	}
	return (len);
}

static	char	*find_env_key(char	*token_word)
{
	t_env	*env;
	int		len;

	len = dollar_len(token_word);
	env = set_env(GET, NULL);
	while (env)
	{
		if (ft_strncmp(env->key, token_word, len) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static char	*handle_dollar(char *token_word, char *new, int *i, t_token *token)
{
	char	*env_key;
	int		len;
	char	*tmp;

	len = 0;
	if (*token_word == '?')
	{
		tmp = ft_itoa(end_status(GET, 0));
		new = strjoin_with_free(new, tmp, FREE_S1);
		return (*i += 1, token->is_dollar = true, free(tmp), new);
	}
	else
	{
		env_key = find_env_key(token_word);
		len = dollar_len(token_word);
		if (!env_key)
			return (*i += dollar_len(token_word), token->is_dollar = true, new);
		else
		{
			new = strjoin_with_free(new, env_key, FREE_S1);
			return (*i += len, token->is_dollar = true, new);
		}
	}
	return (0);
}

char	*expand_dollar(char *tokenized, t_token *token)
{
	int		i;
	char	*new;
	int		single_flag;
	int		double_flag;

	init_variables(&i, &single_flag, &double_flag);
	new = ft_strdup("");
	while (tokenized[i])
	{
		if (tokenized[i] == '\"')
			double_flag++;
		if (tokenized[i] == '\'' && double_flag % 2 == 0)
			single_flag++;
		if (single_flag % 2 == 0 && tokenized[i] == '$')
		{
			if (tokenized[++i])
				new = handle_dollar(&tokenized[i], new, &i, token);
			else
				return (free(tokenized), ft_strjoin_one(new, '$'));
		}
		else
			new = ft_strjoin_one(new, tokenized[i++]);
	}
	return (free(tokenized), new);
}
