/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/25 12:50:25 by user42            #+#    #+#             */
/*   Updated: 2021/02/25 12:53:30 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_basic_env(void)
{
	if ((g_env = malloc(sizeof(char*) * 2)))
	{
		g_env[0] = ft_strdup("PATH=/bin");
		g_env[1] = 0;
	}
	return (g_env);
}
