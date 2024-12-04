/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_open_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 06:49:07 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/27 00:44:55 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*set_file_err(char *filename, char *err_msg)
{
	char	*message;

	message = NULL;
	message = strjoin_with_free("bash: ", filename, NO_FREE);
	message = strjoin_with_free(message, ": ", FREE_S1);
	message = strjoin_with_free(message, err_msg, FREE_S1);
	message = strjoin_with_free(message, "\n", FREE_S1);
	return (message);
}

static bool	heredoc_process(char *eof, t_cmd *cmd)
{
	char	*str;
	int		fd;

	fd = open(FILE_NAME, O_CREAT | O_RDWR, 0644);
	if (fd < 0)
		return (false);
	str = NULL;
	cmd->count = 0;
	while (1)
	{
		ft_printf(1, "> ");
		str = get_next_line(0);
		if (!str)
			return (limitter_warning(cmd->count + 1, eof), close(fd), true);
		(cmd->count)++;
		if (ft_memcmp(str, eof, ft_strlen(eof) + 1) == 10)
			return (free(str), close(fd), true);
		ft_printf(fd, "%s", str);
		free(str);
		str = NULL;
	}
	return (close(fd), true);
}

static bool	open_read_file(t_cmd *cmd, t_token *token)
{
	if (token->kind == LIMITTER)
	{
		if (heredoc_process(token->word, cmd) == false)
			return (false);
		cmd->readfd = open(FILE_NAME, O_RDONLY);
		if (cmd->readfd < 0)
			cmd->err_msg = set_file_err(FILE_NAME, strerror(errno));
		else
			end_status(SET, EXIT_SUCCESS);
	}
	else if (token->kind == RDFILE)
	{
		cmd->readfd = open(token->word, O_RDONLY);
		if (cmd->readfd < 0)
			cmd->err_msg = set_file_err(token->word, strerror(errno));
		else
			end_status(SET, EXIT_SUCCESS);
	}
	return (true);
}

static bool	open_write_file(t_cmd *cmd, t_token *token)
{
	if (token->kind == WRF_APP)
	{
		cmd->writefd = open(token->word, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (cmd->writefd < 0 && cmd->readfd < 0)
			cmd->err_msg = set_file_err(token->word, strerror(errno));
		else
			end_status(SET, EXIT_SUCCESS);
	}
	else if (token->kind == WRFILE)
	{
		cmd->writefd = open(token->word, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (cmd->writefd < 0 && !cmd->err_msg)
			cmd->err_msg = set_file_err(token->word, strerror(errno));
		else
			end_status(SET, EXIT_SUCCESS);
	}
	return (true);
}

void	open_files(t_cmd *cmd, t_token *token)
{
	if (token->kind == RDFILE || token->kind == LIMITTER)
	{
		if (cmd->readfd > 0)
			close(cmd->readfd);
		open_read_file(cmd, token);
	}
	if (token->kind == WRF_APP || token->kind == WRFILE)
	{
		if (cmd->writefd > 0)
			close(cmd->writefd);
		open_write_file(cmd, token);
	}
}
