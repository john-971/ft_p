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

static int              read_until_max(int sock, char *buf, off_t size)
{
	off_t               len;
	off_t                r;

	r = 0;
	while ((long)r < (long)size)
	{
		len = (r == 0) ? 0 : r - 1;
		r += recv(sock, &buf[len], size - r, 0);
		if (r == 0)
			break ;
	}
	return (r);
}

int			send_file(int fd, int sock, off_t f_size, char *type)
{
	int 		r;
	char 		buff[FILE_SIZE + 1];
	t_trame		trame;
	off_t		curr_size;
	int 		i;
	char 		buff2[sizeof(t_trame)];

	curr_size = 0;
	i = 0;
	while ((r = read(fd, buff, FILE_SIZE)) > 0)
	{
//		print_succes("in send file");
		curr_size += r;
		buff[r] = '\0';
		send(sock, buff, r, 0);
		ft_bzero(buff, FILE_SIZE);
//		trame = listen_sock(sock);
		read_until_max(sock, buff2, sizeof(t_trame));
		trame = *(t_trame *)buff2;
//		print_succes("LISTEN COMMAND OK !!!!");
//		printf("CLIENT : %lli  | SERVER : %i\n",trame.size, i);
		if (trame.error == 1)
		{
			print_error("DEBUG : TRAME ERROR\n");
			return (RET_KO);
		}
		if (ft_strcmp(trame.value, ABORT) == 0)
		{
			print_error("SEND FILE ABORT");
			return (RET_KO);
		}
//		else if(ft_strcmp(trame.value, OK) == 0)
//		{
//			print_succes("IS OK !!!!!!!!");
//		}
//		else{
//			print_error("WTF IS NOT OK !!!!!!! ");
//		}

		if (type == T_PUT)
			print_status_bar(curr_size, f_size);
		i++;
	}
	ft_putchar('\n');

	return (RET_OK);
	send_message(T_MSG_OK, GET_OK, sock);
}

int				recev_file(int sock, int fd, off_t f_size, char *type)
{
	char 		buff[FILE_SIZE + 1];
	int			r;
	off_t		curr_size;
	off_t		size_tmp;
	int 		i;

	curr_size = 0;
	i = 0;
	size_tmp = f_size;
	while ((long)curr_size < (long)f_size)
	{
//		printf("IN RECEIV | SIZE_TMP %lld\n", size_tmp);
		if (size_tmp > FILE_SIZE)
//			r = recv(sock, buff, FILE_SIZE, 0);
			r = read_until_max(sock, buff, FILE_SIZE);
		else
			r = read_until_max(sock, buff, size_tmp);
//			r = recv(sock, buff, size_tmp, 0);


		if (r == -1)
			return -1;
		curr_size += r;
		size_tmp -= r;
		if (write(fd, buff, r) == -1)
		{
			print_error(get_error());
			send_command(type, ABORT, sock, 0);
			return -1;
		}
		send_command(type, OK, sock, i);
//		print_succes("SEND COMMAND OK !!!!");
//		printf("%i\n", i);
		if (type == T_GET)
			print_status_bar(curr_size, f_size);
		i++;
	}
	ft_putchar('\n');
	printf("END RECEIV %lld : %lld\n", curr_size, f_size);
	return 0;
}