/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 19:05:39 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 12:48:02 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_error(char *exec_path)
{
	struct stat	file_stat;

	if (stat(exec_path, &file_stat) == -1)
		return (126 + put_error("minishell", exec_path,
		"No such file or directory", 0));
	else if (S_ISDIR(file_stat.st_mode))
		return (125 + put_error("minishell", exec_path, "is a directory", 0));
	return (0);
}

static int	exec_n_wait(char *exec_path, char **argv)
{
	pid_t		pid;
	int			ret;

	if ((pid = fork()) < 0)
		return (1);
	else if (pid == 0)
	{
		signal(SIGTERM, SIG_DFL);
		if (execve(exec_path, argv, g_env) == -1)
		{
			ret = errno;
			put_error("minishell", exec_path, 0, errno);
			exit(126);
		}
	}
	waitpid(pid, &ret, 0);
	if (WIFEXITED(ret))
		ret = WEXITSTATUS(ret);
	return (ret);
}

void		search_n_exec(t_minishell *shell_info, char **argv)
{
	char	*exec_path;
	int		ret;

	ret = 0;
	exec_path = 0;
	if (ft_strchr(argv[0], '/') == 0)
		exec_path = search_path(argv);
	else
		exec_path = ft_strdup(argv[0]);
	if (exec_path == 0)
	{
		shell_info->exit_status = 127;
		return ;
	}
	else if ((shell_info->exit_status = check_error(exec_path)) == 0)
		shell_info->exit_status = exec_n_wait(exec_path, argv);
	free(exec_path);
	return ;
}
