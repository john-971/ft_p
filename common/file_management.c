/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 11:05:25 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/30 11:05:30 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_p.h"

off_t			get_file_size(int fd, int sock)
{
	struct stat buff;

	if (fstat(fd, &buff) == 0)
		return (buff.st_size);
	send_message(T_MSG_KO, get_error(), sock);
	return (-1);
}

static int		read_until_max(int sock, char *buf, off_t size)
{
	off_t		len;
	off_t		r;

	r = 0;
	while ((long)r < (long)size)
	{
		len = (r == 0) ? 0 : r - 1;
		r += recv(sock, &buf[len], size - r, 0);
		if (r == 0 || r == -1)
			break ;
	}
	return (r);
}

int				send_file(int fd, int sock, off_t f_size, char *type)
{
	int			r;
	char		buff[FILE_SIZE + 1];
	t_trame		trame;
	off_t		curr_size;
	char		buff2[sizeof(t_trame)];

	curr_size = 0;
	while ((r = read(fd, buff, FILE_SIZE)) > 0)
	{
		curr_size += r;
		buff[r] = '\0';
		send(sock, buff, r, 0);
		ft_bzero(buff, FILE_SIZE);
		read_until_max(sock, buff2, sizeof(t_trame));
		trame = *(t_trame *)buff2;
		if (trame.error == 1)
			return (RET_KO);
		if (ft_strcmp(trame.value, ABORT) == 0)
			return (RET_KO);
		if (type == T_PUT)
			print_status_bar(curr_size, f_size);
	}
	ft_putchar('\n');
	return (RET_OK);
}

int				manage_res(off_t *size_tmp, off_t *curr_size, int sock, int fd)
{
	char		buff[FILE_SIZE + 1];
	int			r;

	if (*size_tmp > FILE_SIZE)
		r = read_until_max(sock, buff, FILE_SIZE);
	else
		r = read_until_max(sock, buff, *size_tmp);
	if (r == -1)
		return (-1);
	*curr_size += r;
	*size_tmp -= r;
	if (write(fd, buff, r) == -1)
	{
		print_error(get_error());
		send_command(T_GET, ABORT, sock, 0);
		return (-1);
	}
	return (0);
}

int				recev_file(int sock, int fd, off_t f_size, char *type)
{
	off_t		curr_size;
	off_t		size_tmp;

	curr_size = 0;
	size_tmp = f_size;
	while ((long)curr_size < (long)f_size)
	{
		if (manage_res(&size_tmp, &curr_size, sock, fd) == -1)
			return (-1);
		send_command(type, OK, sock, 0);
		if (type == T_GET)
			print_status_bar(curr_size, f_size);
	}
	ft_putchar('\n');
	return (0);
}
