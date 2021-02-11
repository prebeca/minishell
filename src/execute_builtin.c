/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:11:25 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 15:22:24 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd(t_minishell *shell_info, char **argv)
{
	if (argv == 0 || table_len(argv) == 0 || *argv[0] == 0)
	{
		shell_info->exit_status = 0;
		return ;
	}
	errno = 0;
	if (ft_strcmp("echo", argv[0]) == 0)
		shell_info->exit_status = ft_echo(argv);
	else if (ft_strcmp("pwd", argv[0]) == 0)
		ft_pwd();
	else if (ft_strcmp("cd", argv[0]) == 0)
		shell_info->exit_status = ft_cd(argv);
	else if (ft_strcmp("exit", argv[0]) == 0)
		shell_info->exit_status = ft_exit(argv, shell_info);
	else if (ft_strcmp("env", argv[0]) == 0)
		shell_info->exit_status = ft_env();
	else if (ft_strcmp("export", argv[0]) == 0)
		shell_info->exit_status = ft_export(argv);
	else if (ft_strcmp("unset", argv[0]) == 0)
		shell_info->exit_status = ft_unset(argv);
	else
		search_n_exec(shell_info, argv);
}
