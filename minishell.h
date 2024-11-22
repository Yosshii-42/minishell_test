/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:29:52 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/21 13:37:10 by hurabe           ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/includes/libft.h"
# include "./libft/includes/ft_printf.h"
# include "./libft/includes/get_next_line.h"

# define FREE_S1 1
# define FREE_S2 2
# define NO_FREE 0
# define SPECIAL_CHAR "~`#&*()[]{};!?"
# define SPECIAL_TOKEN "<>|$"
# define FILE_NAME "2qryY0jwPY2AXF0VxD2CTIX3uv03Bi"
// シグナル状態の定義
# define READLINE 1
# define HEREDOC 2
# define IN_CMD 3
# define SIG_INT 4
# define ERROR_PRESTR "minishell: "

extern volatile sig_atomic_t	g_sig_status;
extern volatile sig_atomic_t	g_exit_status;

typedef enum e_kind
{
	PIPE,
	COMMAND,
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
}t_kind;

typedef enum e_status
{
	BUILTIN,
	CAT,
	END
}t_status;

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
	char			*err_msg;
	struct s_token	*token;
	t_kind			status;
	int				flag;
}t_cmd;

// env
t_env	*set_env(int argc, char **argv, char **envp, int *statsu);

//token
t_token	*make_token_lst(char *line, t_env *env, int status_num);
void	add_token_kind(t_token *token, int status_num);
t_token	*create_special_token(char **input, t_kind kind, int length);
t_token	*create_command_token(char *start, bool is_quoted, bool is_double_quoted);
t_token *process_quote(char **input, int *error_status, char quote_char);

// lexer.c
t_token	*lexer(t_env *env, char *line, int *error_status);

// tokenizer.c
t_token	*tokenizer(char *input, int *error_status);
int		count_envname_len(char *token, int i);

// tokenizer_utils.c
char	*ft_strjoin_one(char *str, char c);
int		ft_isspace(char c);
char	*ft_strcpy(char *dest, const char *src);
int		ft_strcmp(const char *s1, const char *s2);
bool	is_quote(char c);

// tokenizer_error.c
bool	find_syntax_error(t_token *tokenized);

// expand_token.c
bool	expand_token(t_env *env, t_token *tokenized);
bool	append_char(char **str, char c);

// expand_dollar.c
bool	expand_dollar(t_env *env, t_token *tokenized);
bool	handle_dollar(t_env *env, t_token *tokenized, char **new, int *i);

// expand_quote.c
bool	expand_quote(t_token *tokenized);

// tokenizer_rm_quote.c
bool	remove_quotes(t_token *tokenized);

// command
t_cmd	*make_cmd(t_token *token, t_cmd *cmd, char **path, char *pwd);
void	init_cmd(t_cmd *cmd);
int		count_array(t_token *token);
int		count_token(t_token *token);
int		make_pipe(t_cmd *cmd);
char	*make_pwd_path(char *command, char *pwd);
char	*getenv_str(t_env *env, char *str);

// open
bool	open_files(t_cmd *cmd, t_token *token);

// process
int		run_process(t_token *token, char **path, char *pwd,
			int *original_stdin);

// process utils
int		cmd_count(t_token *token);
int		make_fork(pid_t *pid);
void	exit_child_process(t_cmd *cmd);
void	close_fds(t_cmd *cmd);

// free functions
void	free_env(t_env *env);
void	free_token(t_token *token);
void	free_split(char **split);
void	free_cmd(t_cmd *cmd);

// utils
char	*strjoin_with_free(char *s1, char *s2, int select);
size_t	strchr_len(const char *s, int c);

// signal
void	reset_signal(int signum);
void	ignore_signal(int signum);
void	ready_signal(int signum);
void	init_signal(void);
void	destroy_signal(void);

// signal_handler
int		event(void);
void	perror_prestr(void);
void	fatal_error_exit(char *err_msg);
void	signal_handler(int signum);

// builtin
bool    main_exit(char *line, int *status);
int		builtin_exit(char **split);
void	print_env(t_env *env);
void	print_dolquestion(char *str, int status);
bool	builtin(char *line, t_env *env, int *status);

#endif