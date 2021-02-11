/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_search_path.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:13:41 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 12:49:29 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_path_split(char *path_list)
{
	int		nb_separator;
	char	*iter;
	char	**bin_path;
	t_list	*tmp;

	if ((bin_path = ft_split(path_list, ':')) == 0)
		return (0);
	nb_separator = 0;
	iter = path_list;
	while ((iter = ft_strchr(iter, ':')))
	{
		iter++;
		nb_separator++;
	}
	if (nb_separator >= table_len(bin_path))
	{
		tmp = str_tab_to_lst(bin_path);
		ft_lstadd_back(&tmp, ft_lstnew(ft_strdup(".")));
		free_tab(bin_path);
		bin_path = lst_to_str_tab(tmp);
		ft_lstclear(&tmp, &free);
	}
	return (bin_path);
}

static char	*get_path(char *file_name, char *path_list)
{
	int			i;
	char		*name;
	char		*exec_path;
	char		**bin_path;
	struct stat	file_stat;

	if ((bin_path = get_path_split(path_list)) == 0)
		return (0);
	name = ft_strjoin("/", file_name);
	exec_path = 0;
	i = 0;
	while (bin_path[i])
	{
		if ((exec_path = ft_strjoin(bin_path[i], name)) == 0)
			return (0);
		if (stat(exec_path, &file_stat) == 0)
			break ;
		free(exec_path);
		exec_path = 0;
		++i;
	}
	free(name);
	free_tab(bin_path);
	return (exec_path);
}

char		*search_path(char **argv)
{
	char *path;
	char *exec_path;

	path = ft_getenv("PATH");
	exec_path = get_path(argv[0], path);
	if (exec_path == 0)
	{
		if (path == 0 || path[0] == 0)
			put_error("minishell", argv[0], "No such file or directory", 0);
		else
			put_error("minishell", argv[0], "command not found", 0);
		return (0);
	}
	return (exec_path);
}
