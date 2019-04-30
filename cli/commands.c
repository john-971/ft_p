#include "../includes/ft_p.h"

void					manage_ls(int sock)
{
	char				buff[TRANS_SIZE + 1];
	int					r;
	t_trame				*trame;

	while ((r = recv(sock, buff, TRANS_SIZE, 0)) > 0)
	{
		if (r == -1)
		{
			print_error(ERROR_EXEC);
			return ;
		}
		buff[r] = '\0';
		trame = (t_trame *)buff;
		if (trame->type == T_MSG)
		{
			print_error(trame->value);
			return ;
		}
		else
			printf("%s", buff);
		if(r < TRANS_SIZE)
			break ;
	}
	send_message(T_MSG_OK, OK, sock);
}

int					manage_get(t_trame trame, int sock)
{
	int 			fd;
	int 			ret;

	if ((fd = open(trame.value, O_CREAT | O_RDWR | O_TRUNC, 0777)) != -1)
	{
		send_command(T_GET, OK, sock, 0);
		if (recev_file(sock, fd, trame.size, T_GET) == -1)
			ret = 0;
		else
			ret = 1;
		close(fd);
	}
	else
	{
		send_command(T_GET, ABORT, sock, 0);
		print_error(strerror(errno));
		ret = 0;
	}
	return (ret);
}