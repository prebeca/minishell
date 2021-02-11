/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 14:57:30 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 13:57:57 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*ft_get_var_name(char *var)
{
	char	*name;
	int		len;

	len = ft_strclen(var, '=');
	if ((name = ft_substr(var, 0, len)) == 0)
		return (0);
	return (name);
}

char		*ft_get_var_value(char *var)
{
	int		len;
	int		start;
	char	*value;

	if (ft_strchr(var, '=') == 0)
		return (0);
	start = ft_strclen(var, '=') + 1;
	len = ft_strlen(var + start);
	if ((value = ft_substr(var, start, len)) == 0)
		return (0);
	return (value);
}

char		**ft_find_in_env(char *name)
{
	int		i;
	int		len_a;
	int		len_b;

	i = 0;
	while (g_env[i])
	{
		len_a = ft_strclen(g_env[i], '=');
		len_b = ft_strlen(name);
		len_a = (len_a > len_b ? len_a : len_b);
		if (ft_strncmp(g_env[i], name, len_a) == 0)
			return (&g_env[i]);
		++i;
	}
	return (0);
}

int			ft_add_to_env(char *var)
{
	char	**new_env;

	if ((new_env = malloc(sizeof(char*) * (table_len(g_env) + 2))) == 0)
		return (1);
	ft_bzero(new_env, sizeof(char*) * (table_len(g_env) + 2));
	ft_memcpy(new_env, g_env, sizeof(char*) * table_len(g_env));
	if ((new_env[table_len(g_env)] = ft_strdup(var)) == 0)
	{
		free_tab(new_env);
		return (1);
	}
	free(g_env);
	g_env = new_env;
	return (0);
}

int			ft_remove_from_env(char *name)
{
	int		i;
	int		j;
	char	**new_env;

	if ((new_env = malloc(sizeof(char*) * (table_len(g_env)))) == 0)
		return (1);
	ft_bzero(new_env, sizeof(char*) * (table_len(g_env)));
	i = 0;
	j = 0;
	while (g_env[i])
	{
		if (ft_strncmp(g_env[i], name, ft_strclen(g_env[i], '=')) == 0)
		{
			free(g_env[i]);
			j = 1;
		}
		else
			new_env[i - j] = g_env[i];
		++i;
	}
	free(g_env);
	g_env = new_env;
	return (0);
}
