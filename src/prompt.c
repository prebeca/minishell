/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 16:55:34 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 15:33:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_prompt(void)
{
	char *prompt;

	prompt = ft_strdup("\x1b[32m$USER\x1b[0m:\x1b[34m$PWD\x1b[0m$ ");
	if (prompt && variables_expansions(&prompt, 0) == 0)
		ft_putstr_fd(prompt, STDERR_FILENO);
	else
		ft_putstr_fd("$ ", STDERR_FILENO);
	free(prompt);
	return (0);
}

int	prompt(t_minishell *shell_info)
{
	int		gnl_ret;
	char	*input;

	input = 0;
	while (!shell_info->exit)
	{
		signal(SIGINT, &sigint_handler_prompting);
		signal(SIGQUIT, SIG_IGN);
		print_prompt();
		gnl_ret = get_next_line(STDIN_FILENO, &input);
		signal(SIGQUIT, &sigquit_handler);
		signal(SIGINT, &sigint_handler_executing);
		if (gnl_ret != 1)
		{
			free(input);
			break ;
		}
		shell_info->exit_status = process_input(shell_info, input);
		free(input);
	}
	return (shell_info->exit_status);
}
