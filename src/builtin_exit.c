/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 18:26:35 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 12:46:46 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_arg_before_digit(char *arg, int i, bool *sign)
{
	if (*sign == true && (arg[i] == '+' || arg[i] == '-'))
		return (put_error("minishell: exit", arg,
		"numeric argument required", 0));
	else if (arg[i] == '+' || arg[i] == '-')
		*sign = true;
	if (!ft_strchr(" \t\v\f\r", arg[i]) && *sign == false)
	{
		return (put_error("minishell: exit", arg,
		"numeric argument required", 0));
	}
	return (0);
}

static int	checks_arg(char *arg)
{
	int		i;
	bool	sign;

	i = -1;
	sign = false;
	while (!ft_isdigit(arg[++i]))
		if (check_arg_before_digit(arg, i, &sign))
			return (1);
	while (ft_isdigit(arg[i]))
		++i;
	--i;
	while (arg[++i])
	{
		if (!ft_strchr(BLANKS, arg[i]))
			return (put_error("minishell: exit", arg,
			"numeric argument required", 0));
	}
	return (0);
}

int			ft_exit(char **args, t_minishell *shell_info)
{
	if (table_len(args) >= 2 && checks_arg(args[1]))
	{
		shell_info->exit = true;
		return (255);
	}
	if (table_len(args) > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	shell_info->exit = true;
	if (table_len(args) == 1)
		return (shell_info->exit_status);
	shell_info->exit_status = ft_atoi(args[1]);
	return (shell_info->exit_status);
}
