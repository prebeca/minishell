/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 16:00:59 by user42            #+#    #+#             */
/*   Updated: 2021/02/05 21:29:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	output_string(char **argv, bool option_n)
{
	int		i;
	char	*str;

	str = ft_strdup("");
	i = 0;
	while (argv[i])
	{
		if (ft_strappend(&str, argv[i]))
			return (1);
		if (argv[i + 1] == 0)
			break ;
		if (ft_strappend(&str, " "))
			return (1);
		++i;
	}
	if (option_n)
		ft_putstr_fd(str, STDOUT_FILENO);
	else
		ft_putendl_fd(str, STDOUT_FILENO);
	free(str);
	return (0);
}

static bool	is_valid_option(char *str)
{
	int i;

	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		++i;
	}
	return (true);
}

static char	**read_option(char **argv, bool *option_n)
{
	int i;

	i = 0;
	while (argv[i])
	{
		if (argv[i][0] == '-' && is_valid_option(argv[i]))
			*option_n = true;
		else
			break ;
		++i;
	}
	return (&argv[i]);
}

int			ft_echo(char **argv)
{
	bool	option_n;
	char	**remaining_argv;

	errno = 0;
	option_n = false;
	remaining_argv = read_option(&argv[1], &option_n);
	output_string(remaining_argv, option_n);
	if (errno)
		return (1);
	return (0);
}
