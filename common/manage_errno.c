/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_errno.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 11:05:37 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/30 11:05:42 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_p.h"

char	*get_error()
{
	if (errno == ENOTDIR)
		return (ERR_NOTDIR);
	if (errno == EACCES)
		return (ERR_ACL);
	if (errno == ENOENT)
		return (ERR_ENOENT);
	if (errno == ENAMETOOLONG)
		return (ERR_NAMETOOLONG);
	else
		return (strerror(errno));
}
