/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 16:38:00 by user42            #+#    #+#             */
/*   Updated: 2021/02/15 15:45:23 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_file(t_cmd *cmd, t_list **args)
{
	int		*fd;
	int		flags;
	t_token	*token;

	token = (*args)->content;
	if (ft_strcmp(token->value, O_DGREAT) == 0)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (token->value[0] == O_GREAT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_RDONLY;
	fd = (token->value[0] == O_LESS ? &cmd->fd_input[STDIN_FILENO] :
	&cmd->fd_output[STDOUT_FILENO]);
	token = ((*args)->next)->content;
	if (*fd != -1)
		close(*fd);
	if ((*fd = open(token->value, flags, 0664)) == -1)
	{
		put_error("minishell", token->value, 0, errno);
		if (flags == O_RDONLY)
			return (-1);
		return (1);
	}
	return (0);
}

static void	remove_operator(t_list ***args)
{
	t_list *operator;
	t_list *operand;

	operator = (**args);
	operand = (**args)->next;
	**args = operand->next;
	operand->next = 0;
	ft_lstclear(&operator, &free_token);
}

int			redirection(t_cmd *cmd)
{
	int		ret;
	t_list	**args;

	args = &cmd->args;
	ret = 0;
	while (*args)
	{
		if (((t_token*)((*args)->content))->tag == T_REDIR_OPE)
		{
			ret = open_file(cmd, args);
			remove_operator(&args);
			if (ret < 0)
				return (ret);
			continue ;
		}
		args = &(*args)->next;
	}
	return (ret);
}
