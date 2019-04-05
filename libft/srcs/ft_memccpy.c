/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <jandreu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 14:25:48 by jandreu           #+#    #+#             */
/*   Updated: 2014/11/20 14:04:27 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

void			*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t		i;
	char		*ptr;
	const char	*source;

	i = 0;
	ptr = dst;
	source = src;
	while (i < n)
	{
		if (source[i] == c)
		{
			return (ptr + (i + 1));
		}
		ptr[i] = source[i];
		i++;
	}
	return (NULL);
}
