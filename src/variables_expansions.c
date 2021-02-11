/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_expansions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 17:18:03 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 13:51:37 by user42           ###   ########.fr       */
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
		if (str[i] && (str[i] == '\'' || str[i] == '"' || str[i] == '\\'))
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

static char	*get_var_value(t_minishell *shell_info, char *str, int *j)
{
	char	*var_name;
	char	*var_value;

	while (str[*j] && (ft_isalnum(str[*j])
	|| str[*j] == '_'))
		++(*j);
	if ((var_name = ft_substr(str, 0, *j)) == 0)
		return (0);
	if (*j == 0 && str[0] != '?')
		var_value = ft_strdup("$");
	else if (shell_info && str[0] == '?')
	{
		var_value = ft_itoa(shell_info->exit_status);
		++(*j);
	}
	else if ((var_value = ft_getenv(var_name)) == 0)
		var_value = ft_strdup("");
	else
		var_value = ft_strdup(var_value);
	free(var_name);
	return (var_value);
}

static int	replace_var(char *str, int *i, char **new_str,
t_minishell *shell_info)
{
	int		j;
	char	*var_value;
	char	*tmp;

	var_value = 0;
	++(*i);
	j = 0;
	if ((var_value = get_var_value(shell_info, str + *i, &j)) == 0)
		return (1);
	tmp = 0;
	tmp = var_value;
	if ((var_value = add_quotes(var_value)) == 0)
		return (1);
	free(tmp);
	if ((tmp = ft_strjoin(*new_str, var_value)) == 0)
	{
		free(*new_str);
		return (1);
	}
	free(var_value);
	free(*new_str);
	*new_str = tmp;
	*i += j;
	return (0);
}

static int	variables_expansions_loop(t_minishell *shell_info, char *str,
char **new_str, int *i)
{
	int		j;
	char	*tmp;
	bool	is_escaped;

	j = -1;
	is_escaped = false;
	while (str[*i + ++j])
		if (str[*i + j] == E_ESCAPE && is_escaped == false)
			is_escaped = true;
		else if (str[*i + j] == E_QUOTE && is_escaped == false)
			pass_until_c(&str[*i + j], &j, E_QUOTE);
		else if (str[*i + j] == '$' && is_escaped == false)
		{
			tmp = ft_substr(str, *i, j);
			*i += j;
			ft_strappend(new_str, tmp);
			free(tmp);
			if (replace_var(str, i, new_str, shell_info))
				return (1);
			j = -1;
			continue ;
		}
		else
			is_escaped = false;
	return (0);
}

int			variables_expansions(char **str, t_minishell *shell_info)
{
	int		i;
	char	*new_str;
	char	*tmp;

	i = 0;
	new_str = ft_strdup("");
	if (variables_expansions_loop(shell_info, *str, &new_str, &i))
	{
		free(new_str);
		return (1);
	}
	tmp = ft_substr(*str, i, ft_strlen(*str + i));
	free(*str);
	*str = ft_strjoin(new_str, tmp);
	free(tmp);
	free(new_str);
	return (0);
}
