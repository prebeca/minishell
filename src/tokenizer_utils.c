/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:04:36 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 12:21:55 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				pass_quote(char *input, int *end)
{
	++(*end);
	while (input[*end] && input[*end] != E_QUOTE)
		++(*end);
	if (input[*end] == 0)
	{
		ft_putendl_fd("minishell: unexpected EOF while looking \
for matching `''", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int				pass_d_quote(char *input, int *end)
{
	bool	is_escaped;

	is_escaped = false;
	++(*end);
	while (input[*end])
	{
		if (input[*end] == E_DQUOTE && is_escaped == false)
			break ;
		else if (input[*end] == E_ESCAPE && is_escaped == false)
			is_escaped = true;
		else
			is_escaped = false;
		++(*end);
	}
	if (input[*end] == 0)
	{
		ft_putendl_fd("minishell: unexpected EOF while looking \
for matching `\"'", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int		invalid_token(char *token_value)
{
	char	*error_msg;
	char	*tmp;

	error_msg = "minishell: syntax error near unexpected token `";
	error_msg = ft_strjoin(error_msg, token_value);
	tmp = ft_strjoin(error_msg, "'");
	ft_putendl_fd(tmp, STDERR_FILENO);
	free(error_msg);
	free(tmp);
	return (2);
}

static int		check_invalid_token_loop(t_list **tokens, t_token **last_token)
{
	int		error;
	t_token	*token;

	error = 0;
	while (*tokens && !error)
	{
		token = (*tokens)->content;
		if (token->tag == T_CTRL_OPE && (ft_strcmp(token->value, O_DSEMI) == 0
		|| *last_token == 0))
			error = invalid_token(token->value);
		else if (token->tag == T_CTRL_OPE && ((*last_token == 0
		&& token->value[0] == O_PIPE) || (*last_token != 0
		&& ((*last_token)->tag != T_WORD && (*last_token)->tag != T_OPERAND))))
			error = invalid_token(token->value);
		else if (token->tag == T_REDIR_OPE && *last_token
		&& (*last_token)->tag == T_REDIR_OPE)
			error = invalid_token(token->value);
		if (*last_token != 0 && (*last_token)->tag == T_REDIR_OPE
		&& token->tag == T_WORD)
			token->tag = T_OPERAND;
		*last_token = token;
		*tokens = (*tokens)->next;
	}
	return (error);
}

int				check_invalid_token(t_list *tokens)
{
	t_token	*last_token;
	int		error;

	error = 0;
	last_token = 0;
	error = check_invalid_token_loop(&tokens, &last_token);
	if (!error && last_token && last_token->tag == T_REDIR_OPE)
		error = invalid_token("newline");
	else if (!error && last_token && last_token->tag == T_CTRL_OPE
	&& last_token->value[0] == O_PIPE)
		error = invalid_token(last_token->value);
	return (error);
}
