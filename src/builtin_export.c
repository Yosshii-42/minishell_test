/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-28 07:19:29 by yotsurud          #+#    #+#             */
/*   Updated: 2024-11-28 07:19:29 by yotsurud         ###   ########i         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exchange_value(t_env *env, char *str)
{
	free(env->value);
	env->value = NULL;
	env->value = ft_strdup(ft_strchr(str, '=') + 1);
}

static int	lstnew_export(t_env **start, char *env)
{
	t_env	*new;
	int		len;

	new = (t_env *)safe_malloc(1, sizeof(t_env));
	len = 0;
	if (ft_strchr(env, '='))
		len = strchr_len(env, '=');
	else
		len = ft_strlen(env);
	new->key = (char *)safe_malloc(len + 1, sizeof(char));
	ft_strlcpy(new->key, env, len + 1);
	if (ft_strchr(env, '='))
		new->value = ft_strdup((ft_strchr(env, '=') + 1));
	new->next = NULL;
    printf("lstnew env->key = %s, env->value = %s\n", new->key, new->value);

	lstadd_back(start, new);
	return (TRUE);
}

static void    print_export(void)
{
    t_env   *env;

    env = set_env(GET, NULL);
    while (env)
    {
        ft_printf(1, "declare -x %s", env->key);
        if (env->value)
            ft_printf(1, "=\"%s\"", env->value);
        ft_printf(1, "\n");
        env = env->next;
    }
}

void	builtin_export(t_env **start, t_cmd *cmd)
{
	t_env	*env;
    int     i;

	if (!cmd->cmd[1])
        print_export();
    i = 0;
    while (cmd->cmd[++i])
    {
        if (ft_strchr(cmd->cmd[i], '='))
        {
            env = set_env(GET, NULL);
            while (env)
            {
                if (ft_strncmp(cmd->cmd[i], env->key, strchr_len(cmd->cmd[i], '=')) == 61)
                {
                    exchange_value(env, cmd->cmd[i]);
                    break;
                }
                env = env->next;
            }
            lstnew_export(start, cmd->cmd[i]);
        }
        else
            lstnew_export(start, cmd->cmd[i]);
    }
	end_status(SET, EXIT_SUCCESS);
}

// void	builtin_export(t_cmd *cmd)
// {
// 	t_env	*env;
//     // int     i;

// 	if (!cmd->cmd[1])
//         print_export();
//     // i = 0;
//     (cmd->cmd)++;
//     while (*cmd->cmd)
//     {
//         if (ft_strchr(*cmd->cmd, '='))
//         {
//             env = set_env(GET, NULL);
//             while (env)
//             {
//                 if (ft_strncmp(*cmd->cmd, env->key, strchr_len(*cmd->cmd, '=')) == 61)
//                 {
//                     exchange_value(env, *cmd->cmd);
//                     break;
//                 }
//                 env = env->next;
//             }
//             lstnew_export(&env, *cmd->cmd);
//         }
//         else
//             lstnew_export(&env, *cmd->cmd);
//         (cmd->cmd)++;
//     }
// 	end_status(SET, EXIT_SUCCESS);
// }
