#include "../minishell.h"

static void	set_file_err(t_cmd *cmd, char *filename, char *err_msg)
{
	char	*message;

	message = NULL;
	message = ft_strjoin("bash: ", filename);
	if (!message)
		print_error(strerror(errno));
	message = ft_strjoin2(message, ": ", FREE_S1);
	message = ft_strjoin2(message, err_msg, FREE_S1);
	message = ft_strjoin2(message, "\n", FREE_S1);
	cmd->err_msg = message;
}

void	open_read_file(t_cmd *cmd, t_token *token)
{
	if (token->kind == LIMITTER)
	{
		cmd->readfd = open(FILE_NAME, O_RDONLY);
		if (cmd->readfd < 0)
			set_file_err(cmd, FILE_NAME, strerror(errno));
	}
	else
	{
		cmd->readfd = open(token->word, O_RDONLY);
		if (cmd->readfd < 0)
			set_file_err(cmd, token->word, strerror(errno));
	}
}

void	open_write_file(t_cmd *cmd, t_token *token)
{
	if (token->word == WRFILE_APP)
	{
		cmd->writefd = open(token->word, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (cmd->writefd < 0 && cmd->readfd < 0)
			set_file_err(cmd, token->word, strerror(errno));
	}
	else
	{
		cmd->writefd = open(token->word, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (cmd->writefd < 0 && cmd->readfd < 0)
			set_file_err(cmd, token->word, strerror(errno));
	}
}

void add_token_kind(t_token *token)
{
	t_token	*ptr;

	ptr = token;
	while (ptr)
	{
		if (!ft_memcmp(ptr->word, "|", 2))
		{
			ptr->kind = PIPE;
			if (ptr->next)
				ptr = ptr->next;
		}
		else if (!ft_memcmp(ptr->word, "<", 2))
		{
			ptr->kind = SKIP;
			ptr->next->kind = RDFILE;
			ptr = ptr->next->next;
		}
		else if (!ft_memcmp(ptr->word, "<<", 3))
		{
			ptr->kind = SKIP;
			ptr->next->kind = LIMITTER;
			ptr = ptr->next->next;
		}
		else if (!ft_memcmp(ptr->word, ">", 2))
		{
			ptr->kind = SKIP;
			ptr->next->kind = WRFILE;
			ptr = ptr->next->next;
		}
		else if (!ft_memcmp(ptr->word, ">>", 3))
		{
			ptr->kind = SKIP;
			ptr->next->kind = WRFILE_APP;
			ptr = ptr->next->next;
		}
		else
		{
			if (!*(ptr->word))
				ptr->kind = END;
			else if (!ptr->pre || ptr->pre->kind != COMMAND)
			{
				ptr->kind = COMMAND;
			}
			else if (ptr->pre->kind == COMMAND)
				ptr->kind = ARGUMENT;
			if (ptr->next)
				ptr = ptr->next;
			else
				break;
		}
	}
}
