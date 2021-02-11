/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_expansions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 21:49:50 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 11:55:47 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shell_expansions(t_cmd *cmd, t_minishell *shell_info)
{
	t_list	**tokens;
	t_token	*token;

	tokens = &(cmd->args);
	while (*tokens)
	{
		token = (*tokens)->content;
		if ((token->tag == T_WORD || token->tag == T_OPERAND)
		&& ft_strchr(token->value, '$'))
		{
			if (variables_expansions(&(token->value), shell_info))
				return (1);
			if (token->tag == T_WORD)
			{
				if (word_splitting(&tokens))
					return (1);
			}
			else if (token->tag == T_OPERAND)
				tokens = &(*tokens)->next;
			continue;
		}
		quote_removal(&(token->value));
		tokens = &(*tokens)->next;
	}
	return (0);
}
