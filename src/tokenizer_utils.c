#include "../minishell.h"

bool	is_meta(char c)
{
	return (ft_strchr(SPECIAL_TOKEN, c) != NULL);
}

bool	is_same_top(char *str, char *keyword)
{
	return (ft_memcmp(str, keyword, ft_strlen(keyword)) == 0);
}

bool	is_operand(char	*prompt)
{
	size_t		i;
	const char	*operators[24];

	init_operators(operators);
	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (is_same_top(prompt, operators[i]))
			return (true);
		i++;
	}
	return (false);
}
