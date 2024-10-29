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
		result = (char *)malloc(sizeof(char) * (ft_strlen(s2) + 1));
	else
		result = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!result)
		return (NULL);
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

char	*strjoin_1char(char *s1, char c)
{
	int		i;
	char	*result;

	if (!s1 || !c)
		return (NULL);
	i = -1;
	if (!(*s1))
		result = (char *)malloc(sizeof(char) * 2);
	else
		result = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 2));
	if (!result)
		return (NULL);
	while (s1[++i])
		result[i] = s1[i];
	result[i] = c;
	result[++i] = '\0';
	free(s1);
	return (result);
}