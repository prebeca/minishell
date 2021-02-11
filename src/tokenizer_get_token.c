/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_get_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:06:16 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 12:20:43 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_token_word(char *input, t_token **token)
{
	int		end;
	bool	is_escaped;

	end = 0;
	is_escaped = false;
	while (input[end] && !((ft_strchr(BLANKS, input[end])
	|| ft_strchr(OPERATOR, input[end])) && is_escaped == false))
	{
		if (input[end] == E_ESCAPE && is_escaped == false)
			is_escaped = true;
		else if ((input[end] == E_QUOTE || input[end] == E_DQUOTE)
		&& is_escaped == false)
		{
			if (input[end] == E_DQUOTE && pass_d_quote(input, &end))
				return (2);
			if (input[end] == E_QUOTE && pass_quote(input, &end))
				return (2);
		}
		else
			is_escaped = false;
		++end;
	}
	if ((*token = new_token(ft_substr(input, 0, end), T_WORD)) == 0)
		return (1);
	return (0);
}

static int	get_token_operator(char *input, t_token **token)
{
	if (ft_strncmp(input, O_DGREAT, 2) == 0)
		*token = new_token(ft_substr(input, 0, 2), T_REDIR_OPE);
	else if (*input == O_GREAT)
		*token = new_token(ft_substr(input, 0, 1), T_REDIR_OPE);
	else if (*input == O_LESS)
		*token = new_token(ft_substr(input, 0, 1), T_REDIR_OPE);
	else if (*input == O_PIPE)
		*token = new_token(ft_substr(input, 0, 1), T_CTRL_OPE);
	else if (ft_strncmp(input, O_DSEMI, 2) == 0)
		*token = new_token(ft_substr(input, 0, 2), T_CTRL_OPE);
	else if (*input == O_SEMI)
		*token = new_token(ft_substr(input, 0, 1), T_CTRL_OPE);
	if (*token == 0)
		return (1);
	return (0);
}

int			get_token(char *str, int *i, t_token **token)
{
	int	error;

	error = 0;
	if (ft_strchr(OPERATOR, str[*i]))
		error = get_token_operator(&str[*i], token);
	else
		error = get_token_word(&str[*i], token);
	if (error == 0)
		*i = *i + ft_strlen((*token)->value);
	return (error);
}
