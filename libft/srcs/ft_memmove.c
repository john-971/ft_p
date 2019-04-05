/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <jandreu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/12/01 14:03:06 by jandreu           #+#    #+#             */
/*   Updated: 2014/12/01 14:03:14 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void			*ft_memmove(void *dst, const void *src, size_t len)
{
	void		*tamp;
	const char	*source;
	char		*dest;
	char		*tampon;
	size_t		i;

	source = src;
	dest = dst;
	tamp = (void *)malloc(sizeof(ft_strlen(source) + 1));
	tampon = tamp;
	i = 0;
	if (dst == NULL || src == NULL)
		return (dst);
	while (i < len)
	{
		tampon[i] = source[i];
		i++;
	}
	i = 0;
	while (i < len)
	{
		dest[i] = tampon[i];
		i++;
	}
	return (dst);
}
