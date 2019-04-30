/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 11:06:11 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/30 11:06:13 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_p.h"

char			*get_name_from_path(char *path)
{
	char		*name;

	name = path;
	while ((path = ft_memchr(path, '/', ft_strlen(path))) != NULL)
	{
		path++;
		name = path;
	}
	return (name);
}
