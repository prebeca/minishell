/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 18:54:57 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 11:49:20 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	copy_until_next_quote(char *src, char **dst)
{
	char	*tmp;
	char	*join;

	if ((tmp = ft_substr(src, 0, ft_strclen(src, '\''))) == 0)
		return (1);
	if ((join = ft_strjoin(*dst, tmp)) == 0)
	{
		free(tmp);
		return (1);
	}
	free(tmp);
	free(*dst);
	*dst = join;
	return (0);
}

static void	copy_letter(char c, char **word, bool is_escaped, bool is_d_quoted)
{
	char	*tmp;
	char	letter[2];

	letter[1] = 0;
	if (is_escaped && is_d_quoted && ft_strchr("\\$\"`", c) == 0)
	{
		letter[0] = '\\';
		tmp = *word;
		*word = ft_strjoin(tmp, letter);
		free(tmp);
	}
	is_escaped = false;
	letter[0] = c;
	tmp = *word;
	*word = ft_strjoin(tmp, letter);
	free(tmp);
}

static void	quote_removal_loop(char *str, char **tmp)
{
	int		i;
	bool	is_escaped;
	bool	is_d_quoted;

	is_escaped = false;
	is_d_quoted = false;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\\' && is_escaped == false)
			is_escaped = true;
		else if (str[i] == '\'' && is_escaped == false && is_d_quoted == false)
		{
			copy_until_next_quote(&str[++i], tmp);
			i += ft_strclen(&str[i], '\'');
		}
		else if (str[i] == '\"' && is_escaped == false)
			is_d_quoted = !is_d_quoted;
		else
		{
			copy_letter(str[i], tmp, is_escaped, is_d_quoted);
			is_escaped = false;
		}
	}
}

int			*quote_removal(char **str)
{
	char	*tmp;

	if (ft_strchr(*str, E_ESCAPE) == 0 && ft_strchr(*str, E_QUOTE) == 0
	&& ft_strchr(*str, E_DQUOTE) == 0)
		return (0);
	tmp = ft_strdup("");
	quote_removal_loop(*str, &tmp);
	free(*str);
	*str = tmp;
	return (0);
}
