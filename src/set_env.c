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

static void	lstnew(t_env **start, char *env)
{
	t_env	*new;
	int		len;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		free_env(*start);
	len = 0;
	len = strchr_len(env, '=');
	new->key = (char *)malloc((len + 1) * sizeof(char));
	if (!new->key)
		free_env(*start);
	ft_strlcpy(new->key, env, len + 1);
	new->value = ft_strdup((ft_strchr(env, '=') + 1));
	if (!new->value)
		free_env(*start);
	new->next = NULL;
	lstadd_back(start, new);
}

t_env	*set_env(char **envp)
{
	t_env	*start;
	int	i;

	i = -1;
	start = NULL;
	while (envp[++i])
		lstnew(&start, envp[i]);
	return (start);
}
