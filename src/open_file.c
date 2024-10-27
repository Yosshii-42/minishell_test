#include "../minishell.h"

static char	*set_file_err(char *filename, char *err_msg)
{
	char	*message;

	message = NULL;
	message = strjoin_with_free("bash: ", filename, NO_FREE);
	if (message)
		message = strjoin_with_free(message, ": ", FREE_S1);
    if (message)
		message = strjoin_with_free(message, err_msg, FREE_S1);
    if (message)
		message = strjoin_with_free(message, "\n", FREE_S1);
	if (!message)
		return (ft_printf(2, "%s\n", strerror(errno)), NULL);
	return (message);
}

void	print_limitter_warning(char *eof)
{
	ft_printf(2, "\nbash: warning: here-document delimited by ");
	ft_printf(2, "end-of-file (wanted `%s')\n", eof);
}

static int	here_doc_process(char *eof)
{
	char	*str;
	int		fd;

	fd = open(FILE_NAME, O_CREAT | O_RDWR, 0600);
	if (fd < 0)
		return (FALSE);
	str = NULL;
	while (1)
	{
		ft_printf(1, "> ");
		if (!(str = get_next_line(0)))
		{
			print_limitter_warning(eof);
			break ;
		}
		if (ft_memcmp(str, eof, ft_strlen(eof) + 1) == 10)
		{
			free(str);
			break ;
		}
		ft_printf(fd, "%s", str);
		free(str);
		str = NULL;
	}
	return (close(fd), TRUE);
}

void	open_read_file(t_cmd *cmd, t_token *token)
{
	if (token->kind == LIMITTER)
	{
		if (here_doc_process(token->word) == FALSE)
			return ;
		cmd->readfd = open(FILE_NAME, O_RDONLY);
		if (cmd->readfd < 0)
			cmd->err_msg = set_file_err(FILE_NAME, strerror(errno));
	}
	else if (token->kind == RDFILE)
	{
		cmd->readfd = open(token->word, O_RDONLY);
		if (cmd->readfd < 0)
			cmd->err_msg = set_file_err(token->word, strerror(errno));
	}
}

void	open_write_file(t_cmd *cmd, t_token *token)
{
	if (token->kind == WRFILE_APP)
	{
		cmd->writefd = open(token->word, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (cmd->writefd < 0 && cmd->readfd < 0)
			cmd->err_msg = set_file_err(token->word, strerror(errno));
	}
	else if (token->kind == WRFILE)
	{
		cmd->writefd = open(token->word, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (cmd->writefd < 0 && cmd->readfd < 0)
			cmd->err_msg = set_file_err(token->word, strerror(errno));
	}
}
