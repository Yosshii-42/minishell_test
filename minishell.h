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
// 追加
# include <signal.h>

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
	WRFILE,
	WRF_APP,
	LIMITTER,
	SYNTAX,
	END
} t_kind;

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
	t_kind			status;
	struct s_token	*next;
	struct s_token	*pre;
}t_token;

typedef struct s_cmd
{
	int		readfd;
	int		writefd;
	int		count;
	int		pp[2];
	char	*pathname;
	char	**cmd;
	char	*err_msg;
	struct s_token *token;
	t_kind	status;
}		t_cmd;

// env
t_env	*set_env(char **envp);

//token
t_token	*make_token_lst(char *line);
void 	add_token_kind(t_token *token);

// command
t_cmd	*make_cmd(t_token *token, t_cmd *cmd, char **path, char *pwd);
void	init_cmd(t_cmd *cmd);
int    	make_pipe(t_cmd *cmd);
char	*make_pwd_path(char *command, char *pwd);
char    *getenv_str(t_env *env, char *str);

// file open
void	open_write_file(t_cmd *cmd, t_token *token);
void	open_read_file(t_cmd *cmd, t_token *token);

// process
int		run_process(t_token *token, char **path, char *pwd, int *original_stdin);

// process utils
// int		wait_process(void)
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
// void	print_error_and_exit(char *err_message);

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

#endif