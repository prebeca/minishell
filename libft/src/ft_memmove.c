/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 16:00:39 by prebeca           #+#    #+#             */
/*   Updated: 2020/10/15 12:24:04 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const unsigned char	*scur;
	unsigned char		*dcur;

	if (len <= 0 || src == dst)
		return (dst);
	scur = (const unsigned char*)src;
	dcur = (unsigned char*)dst;
	if (scur < dcur)
		while (len--)
			*(dcur + len) = *(scur + len);
	else
		while (len--)
			*dcur++ = *scur++;
	return (dst);
}
