/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:29:52 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/24 21:45:16 by hurabe           ###   ########.fr       */
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
# define NO_FREE 0
// end_status
# define SET 0
# define GET 1
// # define SPECIAL_CHAR "~`#&*()[]{};!?"
# define SPECIAL_TOKEN "<>|"
# define FILE_NAME "2qryY0jwPY2AXF0VxD2CTIX3uv03Bi"
# define PIPE_EXIST	1
# define NO_PIPE 0
// シグナル状態の定義
# define IN_SHELL 0 // シェルがインタラクティブモードにいる状態
# define READLINE 1 // readlineの入力待ち状態
# define HEREDOC 2 // Here-Documentモード
# define IN_CMD 3  // 外部コマンド実行中
# define SIG_INT 4 // シグナルで中断された
# define ERROR_PRESTR "minishell: "

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
	struct s_token	*token;
	t_kind			status;
	int				flag;
}t_cmd;

typedef struct s_package
{
	t_env	*env;
	t_token	*token;
}t_package;

// status
int end_status(int type, int end_status);

// env
t_env	*set_env(int argc, char **argv, char **envp);

// builtin_cd
void	update_env_var(t_env *env, char *key, char *value);
void	builtin_cd(t_cmd *cmd, t_env *env);

//token
// t_token	*make_token_lst(char *line, int *status);
t_token	*make_token_lst(char *line);
void	add_token_kind(t_token *token);
t_token	*create_special_token(char **input, t_kind kind, int length);
t_token	*create_command_token(char *start, bool is_quoted, bool is_double_quoted);
t_token *process_quote(char **input, int *error_status, char quote_char);

// lexer.c
// t_token	*lexer(char *line, int *error_status);
t_token	*lexer(char *line);

// tokenizer.c
// t_token	*tokenizer(char *input, int *error_status);
t_token	*tokenizer(char *input);
int		count_envname_len(char *token, int i);

// tokenizer_utils.c
char	*ft_strjoin_one(char *str, char c);
int		ft_isspace(char c);
char	*ft_strcpy(char *dest, const char *src);
int		ft_strcmp(const char *s1, const char *s2);
bool	is_quote(char c);
int		check_builtin(char *str);

// tokenizer_error.c
bool	find_syntax_error(t_token *tokenized);

// expand_token.c
// bool	expand_token(t_env *env, t_token *tokenized, int *status);
bool	expand_token(t_env *env, t_token *tokenized);
bool	append_char(char **str, char c);

// expand_dollar.c
// bool	expand_dollar(t_env *env, t_token *tokenized, int *status);
bool	expand_dollar(t_env *env, t_token *tokenized);
// bool	handle_dollar(t_env *env, t_token *tokenized, int *status, int *i);
bool	handle_dollar(t_env *env, t_token *tokenized, int *i);

// expand_quote.c
bool	expand_quote(t_token *tokenized);

// tokenizer_rm_quote.c
bool	remove_quotes(t_token *tokenized);

// command
t_cmd	*make_cmd(t_token *token, t_cmd *cmd, t_env *env);//, char **path);
bool	init_cmd(t_cmd *cmd, t_env *env);
int		count_array(t_token *token);
int		count_token(t_token *token);
int		make_pipe(t_cmd *cmd);
char	*make_pwd_path(char *command, char *pwd);
char	*getenv_str(t_env *env, char *str);
bool	set_err_message(t_cmd *cmd, char *str, char *err_str);

// open
bool	open_files(t_cmd *cmd, t_token *token);

// process
int		run_process(t_token *token, t_env *env, int *stdio);
// int		run_process(t_token *token, t_env *env, int *stdio);

// end process
void	syntax_end(t_cmd *cmd, t_token *token, int stdio[2]);
void	end_process(t_token *token, int stdio[2]);
void	child_exit_process(t_cmd *cmd, int stdio[2]);
int		builtin_end_process(t_cmd *cmd);

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

// signal_process
void	restore_stdio(int *stdio);
void	external_command_signals(void);
void	execute_command(char **argv, char **envp);

// builtin
// bool		do_builtin(t_cmd *cmd, t_env *env, int *status);
// void		builtin_unset(t_cmd *cmd, t_env **env, int *status);
// void		builtin_env(t_env *env, int *status);
// void		builtin_exit(t_cmd *cmd, int *status);
bool		do_builtin(t_cmd *cmd, t_env *env);
void		builtin_unset(t_cmd *cmd, t_env **env);
void		builtin_env(t_env *env);
void		builtin_exit(t_cmd *cmd);

// 仮のもの
bool	print_dolquestion(char *line, int *status);

#endif