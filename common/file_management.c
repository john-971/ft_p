#include "../includes/ft_p.h"

off_t			get_file_size(int fd, int sock)
{
	struct stat buff;


	if (fstat(fd, &buff) == 0)
	{
		printf("DEBUG : GET FILE SIZE => %llu\n", buff.st_size);
		return (buff.st_size);
	}
	send_message(T_MSG_KO, get_error(), sock);
	return (-1);
}

int			send_file(int fd, int sock, off_t f_size, char *type)
{
	int 		r;
	char 		buff[FILE_SIZE + 1];
	t_trame		trame;
	off_t		curr_size;

	curr_size = 0;
	print_succes("in send file");
	while ((r = read(fd, buff, FILE_SIZE)) > 0)
	{
//		print_succes("in send file");
		curr_size += r;
		buff[r] = '\0';
		send(sock, buff, r, 0);
		ft_bzero(buff, FILE_SIZE);
		trame = listen_sock(sock);
		if (trame.error == 1)
		{
			print_error("DEBUG : TRAME ERROR\n");
			return (RET_KO);
		}
		if (trame.value == ABORT)
		{
			print_error("SEND FILE ABORT");
			return (RET_KO);
		}

		if (type == T_PUT)
			print_status_bar(curr_size, f_size);
	}
	print_succes("END OF SEND_FILE");
	ft_putchar('\n');
	return (RET_OK);
	send_message(T_MSG_OK, GET_OK, sock);
}

int				recev_file(int sock, int fd, off_t f_size, char *type)
{
	char 		buff[FILE_SIZE + 1];
	int			r;
	off_t		curr_size;

	curr_size = 0;
	print_succes("in recv file");
	while ((long)curr_size < (long)f_size)
	{
//		printf("IN RECEIV %lu\n", curr_size);
		r = recv(sock, buff, FILE_SIZE, 0);
		if (r == -1)
			return -1;
		curr_size += write(fd, buff, r);
		if (curr_size == -1)
		{
			print_error(get_error());
			send_command(type, ABORT, sock, 0);
			return -1;
		}
		send_command(type, OK, sock, 0);
		if (type == T_GET)
			print_status_bar(curr_size, f_size);
	}
	ft_putchar('\n');
	return 0;
}