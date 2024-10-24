#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

# include <stdio.h>
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

typedef enum e_kind
{
	PIPE,
	COMMAND,
	OPTION,
	SKIP,
	RDFILE,
	WRFILE,
	WRFILE_APP,
	LIMITTER,
	END
} t_kind;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}t_env;

typedef struct s_token
{
	char			*word;
	t_kind			kind;
	t_kind			end;
	struct s_token	*next;
	struct s_token	*pre;
}t_token;

typedef struct s_cmd
{
	int		readfd;
	int		writefd;
	int		pp[2];
	char	*pathname;
	char	**cmd;
	char	*err_msg;
}		t_cmd;

// env
t_env	*set_env(char **envp);

//token
t_token	*make_token_lst(char *line);
void add_token_kind(t_token *token);

// command
t_cmd	*make_cmd(t_token *token, t_cmd *cmd, t_env *env);
void	init_cmd(t_cmd *cmd);
void    safe_pipe(t_cmd *cmd);
char	*make_pwd_path(char *command, char *pwd);
char    *getenv_str(t_env *env, char *str);

// file open
void	open_write_file(t_cmd *cmd, t_token *token);
void	open_read_file(t_cmd *cmd, t_token *token);

// process
int		run_process(char *line, t_env *env);

// process utils
int		cmd_count(t_token *token);
void	make_fork(pid_t *pid);
void	exit_child_process(t_cmd *cmd);
void	close_fds(t_cmd *cmd);
void	token_lstclear(t_token *token);

// free functions
void	free_env(t_env *env);
void	free_env_and_exit(t_env *env);
void	ft_free(char **str, int i);
void	ft_free_split(char **split);
void	ft_free_cmd(t_cmd *cmd);

// utils
char	*strjoin_with_free(char *s1, char *s2, int select);
void	print_error_and_exit(char *err_message);

#endif