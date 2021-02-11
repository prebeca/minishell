/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 17:10:50 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 14:19:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			put_error(char *func_name, char *error_cause,
char *error_desc, int errno_value)
{
	char *output_string;

	output_string = ft_strdup("");
	ft_strappend(&output_string, func_name);
	ft_strappend(&output_string, ": ");
	ft_strappend(&output_string, error_cause);
	if (errno_value != 0 || error_desc != 0)
	{
		ft_strappend(&output_string, ": ");
		if (errno_value != 0)
			ft_strappend(&output_string, strerror(errno_value));
		else if (error_desc != 0)
			ft_strappend(&output_string, error_desc);
		else
			ft_strappend(&output_string, "unknown error");
	}
	if (output_string)
		ft_putendl_fd(output_string, STDERR_FILENO);
	else
		ft_putendl_fd("error", STDERR_FILENO);
	free(output_string);
	return (1);
}

int			ft_strappend(char **dst, char *src)
{
	char	*tmp;

	if (dst == 0)
		return (1);
	if (*dst == 0 && (*dst = ft_strdup("")) == 0)
		return (1);
	tmp = *dst;
	if ((*dst = ft_strjoin(tmp, src)) == 0)
	{
		free(tmp);
		free(*dst);
		*dst = 0;
		return (1);
	}
	free(tmp);
	return (0);
}

static void	swap(char **a, char **b)
{
	char *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void		ft_strsort(char **tab, int first, int last)
{
	int	pivot;
	int i;
	int j;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;
		while (i < j)
		{
			while (ft_strcmp(tab[i], tab[pivot]) < 0 && i < last)
				++i;
			while (ft_strcmp(tab[j], tab[pivot]) > 0)
				--j;
			if (i < j)
				swap(&tab[i], &tab[j]);
		}
		swap(&tab[pivot], &tab[j]);
		ft_strsort(tab, first, j - 1);
		ft_strsort(tab, j + 1, last);
	}
}

char		**t_tokens_to_chartab(t_list *lst)
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
		if ((tab[i] = ft_strdup(((t_token*)lst->content)->value)) == 0)
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
