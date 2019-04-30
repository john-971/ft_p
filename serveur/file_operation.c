/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_operation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 09:29:41 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/30 09:29:44 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_p.h"

int				check_arg(char *arg)
{
	if (ft_strchr(arg, '/') == NULL)
		return (0);
	return (1);
}

int				manage_get(t_trame trame, int fd, int sock, off_t size)
{
	if (trame.error == -1)
	{
		close(fd);
		return (-1);
	}
	if (ft_strcmp(trame.value, OK) == 0)
	{
		if (send_file(fd, sock, size, T_GET) == RET_OK)
			send_message(T_MSG_OK, GET_OK, sock);
		else
			return (-1);
	}
	close(fd);
	return (RET_OK);
}

int				get_command(int sock, t_trame trame)
{
	int			fd;
	off_t		size;

	if (check_arg(trame.value) == 1)
		send_message(T_MSG_KO, ERR_GETPATH, sock);
	else if ((fd = open(trame.value, O_RDWR)) != -1)
	{
		if ((size = get_file_size(fd, sock)) == -1)
			return (RET_KO);
		send_command(T_GET, trame.value, sock, size);
		trame = listen_sock(sock);
		if (manage_get(trame, fd, sock, size) == -1)
			return (-1);
	}
	else
	{
		print_error(get_error());
		send_message(T_MSG_KO, get_error(), sock);
	}
	return (RET_OK);
}

int				put_command(int sock, t_trame trame)
{
	int			fd;
	int			ret;

	if ((fd = open(trame.value, O_CREAT | O_RDWR | O_TRUNC, 0777)) != -1)
	{
		send_command(T_PUT, OK, sock, 0);
		ret = recev_file(sock, fd, trame.size, T_PUT);
		close(fd);
		if (ret == 0)
			send_message(T_MSG_OK, PUT_OK, sock);
		else
			return (-1);
	}
	else
	{
		send_command(T_GET, ABORT, sock, 0);
		print_error(strerror(errno));
	}
	return (0);
}
