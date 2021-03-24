/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 16:22:05 by user42            #+#    #+#             */
/*   Updated: 2021/02/25 12:51:12 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	shlvl_perror(int shlvl)
{
	ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
	ft_putnbr_fd(shlvl, STDERR_FILENO);
	ft_putendl_fd(") too high, resetting to 1", STDERR_FILENO);
	return (1);
}

static void	set_shlvl(void)
{
	int		shlvl;
	char	*tmp_a;
	char	*tmp_b;

	if ((tmp_a = getenv("SHLVL")))
	{
		shlvl = ft_atoi(tmp_a) + 1;
		if (shlvl > 2000000)
			shlvl = shlvl_perror(shlvl);
		shlvl = (shlvl >= 0 ? shlvl : 0);
		tmp_b = ft_itoa(shlvl);
		tmp_a = ft_strjoin("SHLVL=", tmp_b);
		free(tmp_b);
		check_n_export(tmp_a, 0);
		free(tmp_a);
	}
	else
		check_n_export("SHLVL=1", 0);
}

static int	minishell_init(t_minishell *shell_info, char **envp)
{
	char	*tmp;
	char	*cwd;

	signal(SIGINT, &sigint_handler_prompting);
	g_sig_int = false;
	signal(SIGQUIT, SIG_IGN);
	if ((g_env = ft_tabdup(envp)) == 0)
		if (create_basic_env() == 0)
			return (1);
	shell_info->is_interactive = true;
	shell_info->exit_status = 0;
	shell_info->exit = false;
	save_stdio(shell_info->stdio_save);
	set_shlvl();
	cwd = getcwd(NULL, 0);
	if ((tmp = ft_strjoin("PWD=", cwd)) == 0)
	{
		free(cwd);
		return (1);
	}
	check_n_export(tmp, 0);
	free(cwd);
	free(tmp);
	return (0);
}

static int	check_options(t_minishell *shell_info, int argc, char **argv)
{
	if (argc == 1)
		return (0);
	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		shell_info->is_interactive = false;
		return (0);
	}
	return (1);
}

int			minishell(int argc, char **argv, char **envp)
{
	t_minishell	shell_info;

	if (minishell_init(&shell_info, envp) == 1)
		return (-1);
	if (check_options(&shell_info, argc, argv))
		return (1);
	if (shell_info.is_interactive == false)
		shell_info.exit_status = process_input(&shell_info, argv[2]);
	else
	{
		signal(SIGTERM, SIG_IGN);
		prompt(&shell_info);
	}
	free_tab(g_env);
	return (shell_info.exit_status);
}
