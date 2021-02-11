/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 16:27:59 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 13:57:10 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*ft_getenv(char *name)
{
	int		i;
	char	*value;

	i = 0;
	while (g_env[i])
	{
		if (ft_strclen(g_env[i], '=') == ft_strlen(name))
			if (ft_strncmp(g_env[i], name, ft_strclen(g_env[i], '=')) == 0)
			{
				if ((value = ft_strchr(g_env[i], '=')))
					return (value + 1);
				return (value);
			}
		++i;
	}
	return (0);
}

static char	*get_env_output_string(void)
{
	int		i;
	char	*output_string;

	if ((output_string = ft_strdup("")) == 0)
		return (0);
	i = 0;
	while (g_env[i])
	{
		if (ft_strchr(g_env[i], '='))
		{
			if (ft_strappend(&output_string, g_env[i]))
				return (0);
			if (ft_strappend(&output_string, "\n"))
				return (0);
		}
		++i;
	}
	return (output_string);
}

int			ft_env(void)
{
	int		i;
	char	*output_string;

	i = 0;
	output_string = get_env_output_string();
	if (output_string == 0)
		return (1);
	ft_putstr_fd(output_string, STDOUT_FILENO);
	free(output_string);
	return (0);
}
