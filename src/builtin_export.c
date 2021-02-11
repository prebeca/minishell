/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 16:52:19 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 15:29:30 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			check_var_syntaxe(char *var)
{
	int	i;

	if (var == 0)
		return (1);
	i = 0;
	if (!ft_isalpha(var[i]) && var[i] != '_')
		return (1);
	++i;
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (1);
		++i;
	}
	return (0);
}

static int	check_n_export_export(char *var)
{
	char	*name;
	char	**var_address;

	if ((name = ft_get_var_name(var)) == 0)
		return (1);
	if ((var_address = ft_find_in_env(name)))
	{
		free(name);
		if (ft_strchr(var, '=') != 0)
		{
			free(*var_address);
			if ((*var_address = ft_strdup(var)) == 0)
				return (1);
		}
	}
	else
	{
		free(name);
		if (ft_add_to_env(var))
			return (1);
	}
	return (0);
}

int			check_n_export(char *var, int *error)
{
	if (error && check_var_syntaxe(var))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(var, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		*error = 1;
	}
	else
		return (check_n_export_export(var));
	return (0);
}

int			ft_export(char **argv)
{
	int		i;
	int		error;
	char	*output_string;

	if (argv == 0)
		return (-1);
	if (table_len(argv) == 1)
	{
		if ((output_string = get_export_output_string()) == 0)
			return (1);
		ft_putstr_fd(output_string, STDOUT_FILENO);
		free(output_string);
		return (0);
	}
	i = 1;
	error = 0;
	while (argv[i])
	{
		if (check_n_export(argv[i], &error))
			return (1);
		++i;
	}
	return (error);
}
