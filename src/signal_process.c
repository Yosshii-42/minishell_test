/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:01:57 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/24 21:41:14 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_stdio(int *stdio)
{
    // 標準入力を復元
    if (dup2(stdio[0], STDIN_FILENO) < 0)
        ft_printf(2, "Error: %s\n", strerror(errno));

    // 標準出力を復元
    if (dup2(stdio[1], STDOUT_FILENO) < 0)
        ft_printf(2, "Error: %s\n", strerror(errno));

    // 使用したファイルディスクリプタを閉じる
    close(stdio[0]);
    close(stdio[1]);
}

// 外部コマンド実行中のシグナル設定
void	external_command_signals(void)
{
	reset_signal(SIGINT);  // Ctrl+Cをデフォルト動作に戻す
	reset_signal(SIGQUIT); // Ctrl+\をデフォルト動作に戻す
}

void	execute_command(char **argv, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0) //子プロセス
	{
		external_command_signals(); // 子プロセス用のシグナル設定する
		if (execve(argv[0], argv, envp) == -1)
		{
			perror("exec failed");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0) // 親プロセス
	{
		g_sig_status = IN_CMD; // 外部コマンド実行状態に設定する
		if (waitpid(pid, &status, 0) == -1) // 子プロセスの終了を待つ
			perror("waitpid failed");
		g_sig_status = IN_SHELL; // インタラクティブモードに戻る
		init_signal(); // 親プロセスのシグナルを復元する
	}
	else
		perror("fork failed");
}
