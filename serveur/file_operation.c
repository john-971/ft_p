#include "../includes/ft_p.h"

int			check_arg(char *arg)
{
	if (ft_strchr(arg, '/') == NULL)
		return (0);
	return (1);
}

int			get_command(int sock, t_trame trame, t_info *info)
{
	int 		fd;
	off_t		size;

//	printf("DEBUG : get_command => PATH : %s | PATH : %s\n", trame.value, info->base_path);

	if (check_arg(trame.value) == 1)
		send_message(T_MSG_KO, "Merci de donner un nom de fichier et nom un path", sock);
	else if ((fd = open(trame.value, O_RDWR)) != -1)
	{
		print_succes("OPEN IS OK \n");
		if ((size = get_file_size(fd, sock)) == -1)
			return (RET_KO);
		send_command(T_GET, trame.value, sock, size);
		trame = listen_sock(sock);
		if (trame.error == -1)
		{
			printf("DEBUG : TRAME ERROR\n");
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
		else{
			printf("DEBUG : GET COMMAND CLIENT ABORT\n");
		}
		close(fd);
	}
	else
	{
		printf("OPEN FAIL !!!!!!!!!!!!!!\n");
		print_error(get_error());
		send_message(T_MSG_KO, get_error(), sock);
	}
	return (RET_OK);
}

int				put_command(int sock, t_trame trame, t_info *info)
{
	int 			fd;
	int 			ret;

	printf("DEBUG : MANAGE GET => NAME : %s | SIZE : %llu\n", trame.value, trame.size);
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