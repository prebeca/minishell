/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 01:58:06 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 19:22:58 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				pass_until_c(char *input, int *end, char c)
{
	++(*end);
	while (input[*end] && input[*end] != c)
		++(*end);
	return (0);
}

static t_token	*get_token_word(char *input)
{
	int		end;
	bool	is_escaped;

	end = 0;
	is_escaped = false;
	while (input[end] && !(ft_strchr(BLANKS, input[end])
	&& is_escaped == false))
	{
		if (input[end] == E_ESCAPE && is_escaped == false)
			is_escaped = true;
		else if ((input[end] == E_QUOTE || input[end] == E_DQUOTE)
		&& is_escaped == false)
			pass_until_c(input, &end, input[end]);
		else
			is_escaped = false;
		++end;
	}
	return (new_token(ft_substr(input, 0, end), T_WORD));
}

static int		word_splitting_loop(t_list **new_tokens_list, t_token *token)
{
	int		i;
	char	*value;
	t_list	*new_elem;
	t_token	*new_tok;

	i = 0;
	value = token->value;
	while (value[i] && ft_strchr(BLANKS, value[i]))
		++i;
	while (value[i])
	{
		if ((new_tok = get_token_word(&value[i])) == 0)
		{
			ft_lstclear(new_tokens_list, &free_token);
			return (1);
		}
		i += ft_strlen(new_tok->value);
		if (quote_removal(&new_tok->value) || !(new_elem = ft_lstnew(new_tok)))
			return (1);
		ft_lstadd_back(new_tokens_list, new_elem);
		while (value[i] && ft_strchr(BLANKS, value[i]))
			++i;
	}
	return (0);
}

int				word_splitting(t_list ***tokens)
{
	t_token	*token;
	t_list	*new_tokens_list;
	t_list	**tmp;

	new_tokens_list = 0;
	token = (**tokens)->content;
	if (word_splitting_loop(&new_tokens_list, token))
	{
		ft_lstclear(&new_tokens_list, &free_token);
		return (1);
	}
	if (new_tokens_list == 0)
	{
		new_tokens_list = (**tokens)->next;
		ft_lstdelone(**tokens, &free_token);
		**tokens = new_tokens_list;
		return (0);
	}
	tmp = &(ft_lstlast(new_tokens_list)->next);
	ft_lstlast(new_tokens_list)->next = (**tokens)->next;
	ft_lstdelone(**tokens, &free_token);
	**tokens = new_tokens_list;
	*tokens = tmp;
	return (0);
}
