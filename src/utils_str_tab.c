/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 16:20:52 by user42            #+#    #+#             */
/*   Updated: 2021/02/10 19:24:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			table_len(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void		free_tab(char **tab)
{
	int	i;

	if (tab == 0)
		return ;
	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

char		**ft_tabdup(char **tab)
{
	int		i;
	int		len;
	char	**new_tab;

	if (tab == 0)
		return (0);
	len = table_len(tab) + 1;
	if ((new_tab = malloc(sizeof(char*) * len)) == 0)
		return (0);
	ft_bzero(new_tab, sizeof(char*) * len);
	i = 0;
	while (tab[i])
	{
		if ((new_tab[i] = ft_strdup(tab[i])) == 0)
		{
			free_tab(new_tab);
			return (0);
		}
		++i;
	}
	return (new_tab);
}

char		**lst_to_str_tab(t_list *lst)
{
	int		i;
	int		size;
	char	**tab;

	size = ft_lstsize(lst);
	if ((tab = malloc((size + 1) * sizeof(char*))) == 0)
		return (0);
	i = 0;
	while (lst)
	{
		if ((tab[i] = ft_strdup(lst->content)) == 0)
		{
			free_tab(tab);
			return (0);
		}
		++i;
		lst = lst->next;
	}
	tab[i] = NULL;
	return (tab);
}

t_list		*str_tab_to_lst(char **tab)
{
	int		i;
	char	*content;
	t_list	*lst;
	t_list	*new_elem;

	i = 0;
	lst = NULL;
	while (tab && tab[i])
	{
		content = ft_strdup(tab[i]);
		if ((new_elem = ft_lstnew(content)) == 0)
		{
			ft_lstclear(&lst, &free);
			return (0);
		}
		ft_lstadd_back(&lst, new_elem);
		++i;
	}
	return (lst);
}
