#include "../minishell.h"

char	*strjoin_with_free(char *s1, char *s2, int select)
{
	int		i;
	int		j;
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	i = -1;
	j = -1;
	if (!(*s1))
		result = safe_malloc(ft_strlen(s2) + 1, sizeof(char));
	else
		result = safe_malloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	while (s1[++i])
		result[i] = s1[i];
	while (s2[++j])
		result[i + j] = s2[j];
	result[i + j] = '\0';
	if (select == 1 && *s1)
		free(s1);
	else if (select == 2 && *s2)
		free(s2);
	return (result);
}

void	print_error_and_exit(char *err_message)
{
	ft_printf(2, "%s\n", err_message);
	exit(EXIT_FAILURE);
}

void	*safe_malloc(size_t count, size_t size)
{
	void	*result;

	result = NULL;
	result = ft_calloc(count, size);
	if (!result)
		print_error_and_exit(strerror(errno));
	return (result);
}
