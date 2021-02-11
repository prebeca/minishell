/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 15:29:33 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 13:56:10 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_change_pwd(void)
{
	char	*tmp;
	char	*cwd;

	if ((tmp = ft_getenv("PWD")))
	{
		tmp = ft_strjoin("OLDPWD=", tmp);
		check_n_export(tmp, 0);
		free(tmp);
	}
	else
		check_n_export("OLDPWD=", 0);
	cwd = getcwd(NULL, 0);
	tmp = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (tmp)
		check_n_export(tmp, 0);
	free(tmp);
}

int			ft_cd(char **args)
{
	if (table_len(args) > 2)
		put_error("minishell", "cd", "too many arguments", errno);
	if (table_len(args) == 1)
		return (0);
	if (args[1][0] != 0)
		if (chdir(args[1]))
		{
			put_error("minishell: cd", args[1], 0, errno);
			return (1);
		}
	cd_change_pwd();
	return (0);
}
