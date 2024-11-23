/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_open_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-23 06:49:07 by yotsurud          #+#    #+#             */
/*   Updated: 2024-11-23 06:49:07 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	print_limitter_warning(int count, char *eof)
{
	ft_printf(2, "\nbash: warning: here-document delimited at ");
	ft_printf(2, "line%d by end-of-file (wanted `%s')\n", count, eof);
}

static int	heredoc_process(char *eof, t_cmd *cmd)
{
	char	*str;
	int		fd;

	fd = open(FILE_NAME, O_CREAT | O_RDWR, 0600);
	if (fd < 0)
		return (FALSE);
	str = NULL;
	cmd->count = 0;
	while (1)
	{
		ft_printf(1, "> ");
		str = get_next_line(0);
		if (!str)
		{
			print_limitter_warning(cmd->count + 1, eof);
			break ;
		}
		(cmd->count)++;
		if (ft_memcmp(str, eof, ft_strlen(eof) + 1) == 10 && (free(str), 1))
			break ;
		ft_printf(fd, "%s", str);
		free(str);
		str = NULL;
	}
	return (close(fd), TRUE);
}

static bool	open_read_file(t_cmd *cmd, t_token *token)
{
	if (cmd->readfd > 0)
		close(cmd->readfd);
	if (token->kind == LIMITTER)
	{
		if (heredoc_process(token->word, cmd) == FALSE)
			return (false);
		cmd->readfd = open(FILE_NAME, O_RDONLY);
		if (cmd->readfd < 0)
		{
			cmd->err_msg = set_file_err(FILE_NAME, strerror(errno));
			if (!cmd->err_msg)
				return (false);
		}
	}
	else if (token->kind == RDFILE)
	{
		cmd->readfd = open(token->word, O_RDONLY);
		if (cmd->readfd < 0)
		{
			cmd->err_msg = set_file_err(token->word, strerror(errno));
			if (!cmd->err_msg)
				return (false);
		}
	}
	return (true);
}

static bool	open_write_file(t_cmd *cmd, t_token *token)
{
	if (cmd->writefd > 0)
		close(cmd->writefd);
	if (token->kind == WRF_APP)
	{
		cmd->writefd = open(token->word, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (cmd->writefd < 0 && cmd->readfd < 0)
		{
			cmd->err_msg = set_file_err(token->word, strerror(errno));
			if (!cmd->err_msg)
				return (false);
		}
	}
	else if (token->kind == WRFILE)
	{
		cmd->writefd = open(token->word, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (cmd->writefd < 0 && cmd->readfd < 0)
		{
			cmd->err_msg = set_file_err(token->word, strerror(errno));
			if (!cmd->err_msg)
				return (true);
		}
	}
	return (true);
}

bool	open_files(t_cmd *cmd, t_token *token)
{
	if (token->kind == RDFILE || token->kind == LIMITTER)
	{
		if (!open_read_file(cmd, token))
			return (false);
		else
			return (true);
	}
	if (token->kind == WRF_APP || token->kind == WRFILE)
	{
		if (!open_write_file(cmd, token))
			return (false);
		else
			return (true);
	}
	return (false);
}
