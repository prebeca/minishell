/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 14:47:32 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 14:50:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*add_quotes(char *str)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	j = 0;
	while (str[++i])
		if (str[i] == '\'' || str[i] == '"' || str[i] == '\\')
			j++;
	if (!(tmp = malloc(sizeof(char) * (i + j + 1))))
		return (0);
	j = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] && ft_strchr("$\"\\`", str[i]))
		{
			tmp[j] = '\\';
			j++;
		}
		tmp[j] = str[i];
		j++;
	}
	tmp[j] = 0;
	return (tmp);
}

static int	append_value(char *var, char **output_string)
{
	char	*value;
	char	*quoted_value;
	bool	error;

	error = false;
	quoted_value = 0;
	if ((value = ft_get_var_value(var)))
	{
		if (ft_strappend(output_string, "=\""))
			error = true;
		if (!error && (quoted_value = add_quotes(value)) == 0)
			error = true;
		if (!error && ft_strappend(output_string, quoted_value))
			error = true;
		if (!error && ft_strappend(output_string, "\""))
			error = true;
		free(value);
		free(quoted_value);
	}
	return (error);
}

static int	append_var(char *var, char **output_string)
{
	char	*name;

	if (ft_strappend(output_string, "declare -x "))
		return (1);
	if ((name = ft_get_var_name(var)) == 0)
	{
		free(output_string);
		return (1);
	}
	if (ft_strappend(output_string, name))
		return (1);
	free(name);
	if (append_value(var, output_string))
		return (1);
	if (ft_strappend(output_string, "\n"))
		return (1);
	return (0);
}

char		*get_export_output_string(void)
{
	int		i;
	char	*output_string;
	char	**env_cpy;

	env_cpy = ft_tabdup(g_env);
	ft_strsort(env_cpy, 0, table_len(env_cpy) - 1);
	output_string = ft_strdup("");
	i = 0;
	while (env_cpy[i])
	{
		if ((append_var(env_cpy[i], &output_string)))
		{
			free_tab(env_cpy);
			return (0);
		}
		++i;
	}
	free_tab(env_cpy);
	return (output_string);
}
