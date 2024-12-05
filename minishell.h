/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:29:52 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/05 09:07:22 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/includes/libft.h"
# include "./libft/includes/ft_printf.h"
# include "./libft/includes/get_next_line.h"
// strjoin_with_free
# define FREE_S1 1
# define FREE_S2 2
# define FREE_ALL 3
# define NO_FREE 0
// set & get function
# define SET 0
# define GET 1
// safe function
# define CHILD 0
# define PARENT 1
# define EXIT_CHILD_DUP_ERR -2
// # define SPECIAL_CHAR "~`#&*()[]{};!?"
# define SPECIAL_TOKEN "<>|"
# define FILE_NAME "2qryY0jwPY2AXF0VxD2CTIX3uv03Bi"
# define PIPE_EXIST	1
# define NO_PIPE 0

extern volatile sig_atomic_t	g_sig_status;

typedef enum e_kind
{
	PIPE,
	COMMAND,
	BUILTIN,
	OPTION,
	LESSTHAN,
	HERE_DOC,
	MORETHAN,
	APPEND,
	RDFILE,
	LIMITTER,
	WRFILE,
	WRF_APP,
	SYNTAX,
	_NULL
}t_kind;

typedef enum e_status
{
	CAT,
	END
}t_status;

typedef enum e_built
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}t_built;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*pre;
}t_env;

typedef struct s_token
{
	char			*word;
	t_kind			kind;
	t_status		status;
	bool			is_dollar;
	bool			is_quoted;
	bool			is_double_quoted;
	struct s_token	*next;
	struct s_token	*pre;
}t_token;

typedef struct s_cmd
{
	int				readfd;
	int				writefd;
	int				count;
	int				pp[2];
	char			*pathname;
	char			**cmd;
	char			**path;
	char			*err_msg;
	char			*err_file;
	int				err_no;
	struct s_token	*token;
	t_kind			status;
	int				flag;
}t_cmd;

typedef struct s_package
{
	t_env	*env;
	t_token	*token;
}t_package;

// gloval like function
int		end_status(int type, int end_status);
char	*set_line(int type, char *new_line);
t_env	*set_env(int type, t_env *new_env);
t_token	*set_token(int type, t_token *new_token);

// env
t_env	*make_env(int argc, char **argv, char **envp);
int		lstnew(t_env **start, char *env);
t_env	*lstlast(t_env *lst);
void	lstadd_back(t_env **start, t_env *new);

// tokenizer_lexer.c
t_token	*lexer(char *line);

// tokenizer.c
t_token	*tokenizer(char *input);
void	append_token(char *input, int token_len, t_token **head, t_token *new);

// tokenizer_error.c
bool	find_syntax_error(t_token *tokenized);

// tokenizer_expand_dollar.c
char	*expand_dollar(char *tokenized, t_token *token);

// tokenizer_expand_quote.c
char	*expand_quote(char *tokenized, t_token *token);

// tokenizer_lexer_add_kind.c
t_token	*add_kind_pipe(t_token *token);
t_token	*add_kind_lessthan(t_token *token);
t_token	*add_kind_morethan(t_token *token);
t_token	*add_command_kind(t_token *token, int command_flag);
int		check_builtin(char *str);

// tokenizer_utils1.c
char	*space_skip(char *input);
int		count_word_len(char *input, t_token *new);
int		count_meta_len(char *line);
int		count_envname_len(char *token, t_token *new, int start);
void	init_variables(int *i, int *single_flag, int *double_flag);

// tokenizer_utils2.c
char	*ft_strjoin_one(char *str, char c);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_isspace(char c);
bool	is_quote(char c);

// command
t_cmd	*make_cmd(t_token *token, t_cmd *cmd, int command_flag);
bool	init_cmd(t_cmd *cmd);
int		count_array(t_token *token);
int		count_token(t_token *token);
void	make_pipe(t_cmd *cmd);
char	*make_pwd_path(char *command, char *pwd);
char	*getenv_str(char *str);
t_cmd	*command_return(t_cmd *cmd, t_token *token);
void	set_err_message(t_cmd *cmd, char *str, char *err_str);
void	limitter_warning(int count, char *eof);

// open_files
void	open_files(t_cmd *cmd, t_token *token);

// process
int		run_process(t_token *token, int *stdio, int command_count);
int		parent_process(t_cmd *cmd, int count);//t_token *token, int count);

// end process
void	syntax_end(t_cmd *cmd, int stdio[2]);
void	end_process(int stdio[2]);
void	child_exit_process(t_cmd *cmd, int stdio[2]);
int		builtin_end_process(t_cmd *cmd);//, t_token *token);
int		no_fork_process(t_cmd *cmd, int *stdio);
void	execve_fail_process(t_cmd *cmd);

// process utils
int		cmd_count(t_token *token);
int		pipe_count(t_token *token);
int		make_fork(pid_t *pid);
void	close_fds(t_cmd *cmd);

// free functions
void	free_env(t_env *env);
void	free_token(t_token *token);
void	free_split(char **split);
void	free_cmd(t_cmd *cmd);
void	free_all(t_cmd *cmd);

// utils
char	*strjoin_with_free(char *s1, char *s2, int select);
size_t	strchr_len(const char *s, int c);

void	*safe_malloc(size_t count, size_t size);
int		safe_dup(int fd);
void	safe_dup2(int fd, int new_fd, int who, t_cmd *cmd);
void	safe_close(int *fd, int who, t_cmd *cmd);

// signal
int		event(void);
void	core_dump_signal(int status);
void	heredoc_signal(void);
void	exec_child_signal(void);
void	init_signal(void);

// signal_handler
void	heredoc_handler(int signum);
void	sig_handler(int signum);

// builtin
bool	do_builtin(t_cmd *cmd);
void	builtin_cd(t_cmd *cmd, t_env *env);
void	builtin_unset(t_cmd *cmd, t_env **env);
void	builtin_env(void);
void	builtin_exit(t_cmd *cmd);
void	builtin_export(t_env **env, t_cmd *cmd);

#endif