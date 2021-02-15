/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 17:56:18 by user42            #+#    #+#             */
/*   Updated: 2021/02/15 15:45:21 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token		*new_token(char *new_value, t_token_tag new_tag)
{
	t_token	*token;

	if ((token = malloc(sizeof(t_token))) != 0)
	{
		token->value = new_value;
		token->tag = new_tag;
	}
	else
		ft_putendl_fd("minishell: malloc error", STDERR_FILENO);
	return (token);
}

void		free_token(void *void_token)
{
	t_token	*token;

	token = void_token;
	free(token->value);
	free(token);
}

static int	tokenizer_loop(char *input, t_list **tokens)
{
	int		i;
	int		error;
	t_list	*new;
	t_token	*token;

	new = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_strchr(BLANKS, input[i]))
			++i;
		if (input[i] == 0)
			break ;
		token = 0;
		if ((error = get_token(input, &i, &token))
		|| (new = ft_lstnew(token)) == 0)
		{
			free(token);
			if (error == 0 && ++error)
				ft_putendl_fd("minishell: malloc error", STDERR_FILENO);
			return (error);
		}
		ft_lstadd_back(tokens, new);
	}
	return (0);
}

int			tokenizer(char *input, t_list **tokens)
{
	int		error;

	error = 0;
	if ((error = tokenizer_loop(input, tokens)))
		ft_lstclear(tokens, &free_token);
	else if ((error = check_invalid_token(*tokens)))
		ft_lstclear(tokens, &free_token);
	return (error);
}
