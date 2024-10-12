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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}t_env;

typedef struct s_cmd
{
	// int		readfd;
	// int		writefd;
	int		pp[2];
	char	*pathname;
	char	**cmd;
	char	*err_msg;
}		t_cmd;

// env
t_env	*set_env(char **envp);

// command
t_cmd	*make_cmd(char *line, t_env *env, int i);
void	init_cmd(t_cmd *cmd);
void    safe_pipe(t_cmd *cmd);
char	*make_pwd_path(char *command);

// process
int		run_process(char *line, t_env *env);
void	exit_child_process(t_cmd *cmd);
void	close_fds(t_cmd *cmd);

// free functions
void	free_env(t_env *env);
void	ft_free(char **str, int i);
void	ft_free_split(char **split);
void	ft_free_cmd(t_cmd *cmd);

// utils
char	*strjoin_with_free(char *s1, char *s2, int select);
void	print_error_and_exit(char *err_message);
void	*safe_malloc(size_t count, size_t size);

#endif