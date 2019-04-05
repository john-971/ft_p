/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <jandreu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/13 13:50:29 by jandreu           #+#    #+#             */
/*   Updated: 2014/12/08 15:08:33 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	if (s2[i] == '\0')
		return ((char*)s1);
	while (s1[i] && i < n)
	{
		if (j == ft_strlen(s2))
			return ((char*)s1 + i - j);
		if (s1[i] == s2[j])
		{
			i++;
			j++;
			if (j == ft_strlen(s2))
				return ((char*)s1 + i - j);
		}
		else
		{
			i++;
			j = 0;
		}
	}
	return (NULL);
}
