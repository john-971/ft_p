/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <jandreu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/18 11:14:27 by jandreu           #+#    #+#             */
/*   Updated: 2015/01/08 02:16:12 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int		ft_part_len(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

int		ft_part_num(char const *s, char c)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (s[i])
	{
		if ((i == 0 && s[i] != c) || (s[i] != c && s[i - 1] == c))
			nb++;
		i++;
	}
	return (nb + 1);
}

char	**ft_creatab(char **tab, char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if ((i == 0 && s[i] != c) || (s[i] != c && s[i - 1] == c))
		{
			tab[j] = malloc(sizeof(*tab) * ft_part_len(s + i, c));
			if (tab[j] != NULL)
			{
				ft_memcpy(tab[j], s + i, ft_part_len(s + i, c));
			}
			j++;
		}
		i++;
	}
	tab[j] = 0;
	return (tab);
}

char	**ft_strsplit(char const *s, char c)
{
	char	**tab;
	int		nb;

	tab = NULL;
	if (s == NULL || c == 0)
	{
		tab = malloc(sizeof(tab));
		if (tab != NULL)
		{
			*tab = malloc(sizeof(*tab));
			if (*tab != NULL)
				tab[0][0] = '\0';
		}
		return (tab);
	}
	nb = ft_part_num(s, c);
	tab = malloc(sizeof(tab) * nb);
	if (tab != NULL)
		tab = ft_creatab(tab, s, c);
	return (tab);
}
