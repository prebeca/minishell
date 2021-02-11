/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_cmdlist.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:31:47 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 12:04:00 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			free_cmd(void *void_cmd)
{
	t_cmd	*cmd;

	cmd = void_cmd;
	ft_lstclear(&cmd->args, &free_token);
	free(cmd->args);
	free_tab(cmd->args_tab);
	free(cmd);
}

static int		init_cmd(t_cmd *cmd)
{
	if (cmd == 0)
		return (1);
	cmd->args = 0;
	cmd->ctrl_ope = 0;
	cmd->fd_input[0] = -1;
	cmd->fd_input[1] = -1;
	cmd->fd_output[0] = -1;
	cmd->fd_output[1] = -1;
	cmd->pid = -1;
	cmd->is_async = false;
	cmd->args_tab = 0;
	return (0);
}

static t_list	*new_cmd(t_list *tokens)
{
	int		size;
	t_list	*new_elem;
	t_list	**token_iter;
	t_token	*token;
	t_cmd	*cmd;

	if (init_cmd(cmd = malloc(sizeof(t_cmd))))
		return (0);
	token = ft_lstlast(tokens)->content;
	if (token->tag == T_CTRL_OPE)
	{
		cmd->ctrl_ope = token->value[0];
		size = ft_lstsize(tokens) - 1;
		token_iter = &tokens;
		while (size--)
			token_iter = &(*token_iter)->next;
		ft_lstdelone(ft_lstlast(*token_iter), &free_token);
		*token_iter = 0;
	}
	else
		cmd->ctrl_ope = 0;
	cmd->args = tokens;
	if ((new_elem = ft_lstnew(cmd)) == 0)
		free(cmd);
	return (new_elem);
}

static int		get_cmd(t_list **cmd_list, t_list **arg_start)
{
	t_list	*new_elem;

	if ((new_elem = new_cmd(*arg_start)) == 0)
	{
		ft_lstclear(cmd_list, &free_cmd);
		ft_lstclear(arg_start, &free_token);
		ft_putendl_fd("minishell: malloc error", STDERR_FILENO);
		return (1);
	}
	ft_lstadd_back(cmd_list, new_elem);
	return (0);
}

int				token_to_cmdlist(t_list *tokens, t_list **cmd_list)
{
	t_list	*arg_start;
	t_list	*tmp;

	arg_start = tokens;
	while (tokens)
	{
		if (((t_token*)tokens->content)->tag == T_CTRL_OPE)
		{
			tmp = tokens->next;
			tokens->next = 0;
			if (get_cmd(cmd_list, &arg_start))
				return (1);
			arg_start = tmp;
			tokens = arg_start;
			continue ;
		}
		tokens = tokens->next;
	}
	if (arg_start != tokens)
		if (get_cmd(cmd_list, &arg_start))
			return (1);
	return (0);
}
