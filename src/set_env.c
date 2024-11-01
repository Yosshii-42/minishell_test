#include "../minishell.h"

static t_env	*lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

static void	lstadd_back(t_env **start, t_env *new)
{
	t_env	*ptr;

	ptr = NULL;
	if (!*start)
	{
		*start = new;
		return ;
	}	
	ptr = lstlast(*start);
	ptr->next = new;
}

static size_t	strchr_len(const char *s, int c)
{
	size_t			i;
	size_t			len;
	unsigned char	cc;

	i = 0;
	len = 0;
	cc = (unsigned char)c;
	len = ft_strlen(s);
	while (s[i] != cc && i < len)
		i++;
	if (s[i] == cc && i <= len)
		return (i);
	else
		return (0);
}

static int	lstnew(t_env **start, char *env)//25行制限
{
	t_env	*new;
	int		len;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (FALSE);
	len = 0;
	len = strchr_len(env, '=');
	new->key = (char *)malloc((len + 1) * sizeof(char));
	if (!new->key)
		return (free(new), FALSE);
	ft_strlcpy(new->key, env, len + 1);
	new->value = ft_strdup((ft_strchr(env, '=') + 1));
	if (!new->value)
		return (free(new->key), free(new), FALSE);
	new->next = NULL;
	lstadd_back(start, new);
	return (TRUE);
}

t_env	*set_env(char **envp)
{
	t_env	*start;
	int	i;

	i = -1;
	start = NULL;
	while (envp[++i])
	{
		if (!lstnew(&start, envp[i]))
			return (free_env(start), NULL);
	}
	return (start);
}
