#include "../minishell.h"

static t_token	*lstlast(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

static void	lstadd_back(t_token **start, t_token *new)
{
	t_token	*ptr;

	ptr = NULL;
	if (!*start)
	{
		*start = new;
		new->pre = NULL;
		return ;
	}
	ptr = lstlast(*start);
	ptr->next = new;
	new->pre = ptr;
}

static void lstnew(t_token **start, char *token)
{
	t_token *new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return ;
	new->word = ft_strdup(token);
	if (!(new->word))
		return ;
	new->kind = -1;
	new->end = -1;
	new->next = NULL;
	lstadd_back(start, new);
}

t_token	*make_token_lst(char *line)
{
	char	**split;
	int		i;
	t_token	*token;
	split = ft_split(line, ' ');
	if (!split)
		return (NULL);
	token = NULL;
	i = -1;
	while (split[++i])
		lstnew(&token, split[i]);
	add_token_kind(token);
	ft_free_split(split);
    free(line);
	return (token);
}
