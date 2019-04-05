/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <jandreu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/18 08:54:00 by jandreu           #+#    #+#             */
/*   Updated: 2015/01/08 02:17:06 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char				*ft_strtrim(char const *s)
{
	unsigned char	*str;
	int				len;
	int				i;
	int				j;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	len = ft_strlen((char*)s);
	str = (unsigned char*)malloc(sizeof(str) * len + 1);
	if (!str)
		return (NULL);
	while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t')
		i++;
	while (s[len - 1] == ' ' || s[len - 1] == '\n' || s[len - 1] == '\t')
		len--;
	while (i < len)
	{
		str[j] = s[i];
		i++;
		j++;
	}
	str[i] = '\0';
	return ((char*)str);
}
