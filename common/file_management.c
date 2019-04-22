#include "../includes/ft_p.h"

off_t			get_file_size(int fd, int sock)
{
	off_t		size;
	struct stat buff;


	if (fstat(fd, &buff) == 0)
	{
		printf("DEBUG : GET FILE SIZE => %llu\n", buff.st_size);
		return (buff.st_size);
	}
	send_message(T_MSG_KO, get_error(), sock);
	return (-1);
}

void			send_file(int fd, int sock)
{
	int 		r;
	char 		buff[FILE_SIZE + 1];
	t_trame		trame;

	printf("DEBUG : GET COMMAND CLIENT SAID OK\n");
	while ((r = read(fd, buff, FILE_SIZE)) > 0)
	{
		buff[r] = '\0';
//		printf("DEBUG : read size => %i \n %s\n", r, buff);
		send(sock, buff, r, 0);
		ft_bzero(buff, FILE_SIZE + 1);

		trame = listen_sock(sock);
		if (trame.error == -1)
			exit(EXIT_FAILURE);
		if (trame.value == ABORT)
			return ;
	}
	send_message(T_MSG_OK, GET_OK, sock);
}

int				recev_file(int sock, int fd, off_t f_size)
{
	t_trame		file_trame;
	char 		buff[FILE_SIZE + 1];
	int			r;
	off_t		curr_size;

	while ((long)c_size < (long)f_size)
	{
		r = recv(sock, buff, FILE_SIZE, 0);
		c_size += write(fd, buff, r);
		if (c_size == -1)
		{
			print_error(get_error());
			send_command(T_GET, ABORT, sock, 0);
			return -1;
		}
		send_command(T_GET, OK, sock, 0);
		print_status_bar(c_size, f_size);
	}
	ft_putchar('\n');
	return 0;
}