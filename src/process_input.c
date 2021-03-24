/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 17:19:36 by user42            #+#    #+#             */
/*   Updated: 2021/02/25 12:58:11 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_childs(t_minishell *shell_info, t_list *cmd_lst)
{
	pid_t		pid;
	t_cmd		*cmd;
	t_list		*cmd_iter;

	while ((pid = waitpid(0, &shell_info->exit_status, 0)) > 0)
	{
		cmd_iter = cmd_lst;
		while (cmd_iter)
		{
			cmd = cmd_iter->content;
			if (cmd->pid == pid)
			{
				cmd->pid = -1;
				if (cmd->ctrl_ope != '|')
					shell_info->exit_status =
					WEXITSTATUS(shell_info->exit_status);
				break ;
			}
			cmd_iter = cmd_iter->next;
		}
	}
}

static int	open_pipe(t_list *cmd_lst)
{
	t_cmd	*cmd;
	t_cmd	*next_cmd;
	int		pipe_fd[2];

	cmd = cmd_lst->content;
	next_cmd = cmd_lst->next->content;
	pipe(pipe_fd);
	cmd->fd_output[0] = pipe_fd[0];
	cmd->fd_output[1] = pipe_fd[1];
	next_cmd->fd_input[0] = pipe_fd[0];
	next_cmd->fd_input[1] = pipe_fd[1];
	return (0);
}

static int	redirect_n_exec(t_minishell *shell_info, t_list **cmd_iter,
t_cmd *cmd, char last_cmd_ope)
{
	if ((shell_info->exit_status = redirection(cmd)) < 0)
	{
		shell_info->exit_status = 1;
		reset_stdio(shell_info->stdio_save);
		last_cmd_ope = cmd->ctrl_ope;
		*cmd_iter = (*cmd_iter)->next;
		return (1);
	}
	cmd->args_tab = t_tokens_to_chartab(cmd->args);
	if (cmd->ctrl_ope == O_PIPE || last_cmd_ope == O_PIPE)
		cmd->pid = fork();
	if (cmd->pid > 0)
		return (0);
	if (cmd->fd_input[STDIN_FILENO] != -1)
		dup2(cmd->fd_input[STDIN_FILENO], STDIN_FILENO);
	if (cmd->fd_output[STDOUT_FILENO] != -1)
		dup2(cmd->fd_output[STDOUT_FILENO], STDOUT_FILENO);
	close_pipe(cmd->fd_input);
	close_pipe(cmd->fd_output);
	execute_cmd(shell_info, cmd->args_tab);
	if (cmd->pid == 0)
		return ((shell_info->exit = true));
	return (0);
}

static int	input_execution_loop(t_minishell *shell_info, t_list *cmd_list)
{
	char	last_cmd_ope;
	t_list	*cmd_iter;

	last_cmd_ope = 0;
	cmd_iter = cmd_list;
	while (cmd_iter && !shell_info->exit && !g_sig_int)
	{
		if (shell_expansions(((t_cmd*)cmd_iter->content), shell_info))
			return (shell_info->exit_status);
		if (((t_cmd*)cmd_iter->content)->ctrl_ope == O_PIPE)
			open_pipe(cmd_iter);
		if (redirect_n_exec(shell_info, &cmd_iter,
		((t_cmd*)cmd_iter->content), last_cmd_ope))
			continue ;
		if (((t_cmd*)cmd_iter->content)->fd_input[STDIN_FILENO] != -1)
			close(((t_cmd*)cmd_iter->content)->fd_input[STDIN_FILENO]);
		if (((t_cmd*)cmd_iter->content)->fd_output[STDOUT_FILENO] != -1)
			close(((t_cmd*)cmd_iter->content)->fd_output[STDOUT_FILENO]);
		if (((t_cmd*)cmd_iter->content)->ctrl_ope != O_PIPE)
			wait_childs(shell_info, cmd_list);
		reset_stdio(shell_info->stdio_save);
		last_cmd_ope = ((t_cmd*)cmd_iter->content)->ctrl_ope;
		cmd_iter = cmd_iter->next;
	}
	return (0);
}

int			process_input(t_minishell *shell_info, char *input)
{
	int		error;
	t_list	*tokens;
	t_list	*cmd_list;

	tokens = 0;
	error = 0;
	cmd_list = 0;
	if ((error = tokenizer(input, &tokens)) || tokens == 0)
		return (error);
	if ((error = token_to_cmdlist(tokens, &cmd_list)))
		return (error);
	input_execution_loop(shell_info, cmd_list);
	g_sig_int = false;
	ft_lstclear(&cmd_list, &free_cmd);
	return (shell_info->exit_status);
}
