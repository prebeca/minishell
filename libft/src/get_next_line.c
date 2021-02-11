/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 13:17:24 by prebeca           #+#    #+#             */
/*   Updated: 2021/01/07 15:00:02 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_substr_gnl(char const *s, unsigned int start, size_t len)
{
	char			*substr;
	unsigned long	slen;

	if (!s)
		return (0);
	slen = ft_strclen(s, '\0');
	start = (start > slen) ? slen : start;
	len = (start + len > slen) ? slen - start : len;
	substr = 0;
	if (!(substr = malloc((len + 1) * sizeof(char))))
		return (0);
	ft_strlcpy(substr, s + start, len + 1);
	return (substr);
}

int			check_eof(int fd, char **line, char buffer[MAX_FD][BUFFER_SIZE + 1])
{
	char *tmp;

	if (!(tmp = ft_substr_gnl(*line, 0, ft_strclen(*line, '\n'))))
	{
		free(*line);
		return (-1);
	}
	free(*line);
	*line = tmp;
	if ((tmp = ft_strchr(buffer[fd], '\n')))
		ft_strlcpy(buffer[fd], buffer[fd] + ft_strclen(buffer[fd], '\n') + 1,
				BUFFER_SIZE + 1);
	else
	{
		buffer[fd][0] = 0;
		return (0);
	}
	return (1);
}

int			get_next_line(int fd, char **line)
{
	static char buffer[MAX_FD][BUFFER_SIZE + 1];
	char		*tmp;
	int			ret;

	if (fd < 0 || line == 0 || BUFFER_SIZE <= 0 || read(fd, buffer[fd], 0))
		return (-1);
	*line = 0;
	ret = 0;
	if (!(*line = ft_substr_gnl(buffer[fd], 0, ft_strclen(buffer[fd], '\0'))))
		return (-1);
	if (!ft_strchr(buffer[fd], '\n'))
		while ((ret = read(fd, buffer[fd], BUFFER_SIZE)))
		{
			buffer[fd][ret] = 0;
			if (!(tmp = ft_strjoin(*line, buffer[fd])))
			{
				free(*line);
				return (-1);
			}
			free(*line);
			*line = tmp;
			if (ft_strchr(buffer[fd], '\n'))
				break ;
		}
	return (check_eof(fd, line, buffer));
}
