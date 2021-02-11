/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 15:13:16 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 17:11:06 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler_prompting(int num)
{
	(void)num;
	ft_putchar_fd('\n', STDERR_FILENO);
	print_prompt();
}

void	sigint_handler_executing(int num)
{
	(void)num;
	g_sig_int = true;
}

void	sigquit_handler(int num)
{
	(void)num;
	ft_putendl_fd("Quit (core dump)", STDERR_FILENO);
}
