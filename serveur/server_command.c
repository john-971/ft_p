#include "../includes/ft_p.h"

void				ls_command(int sock)
{
	char 			cwd[PATH_MAX];
	char 			*test[3];
	int 			child;
	int 			exit_status;

	exit_status = -1;
	test[0] = "ls";
	test[1] = "-l";
	test[2] = NULL;
	if ((child = fork()) > 0)
	{
		wait4(child, &exit_status, 0, NULL);
		if(exit_status == RET_EXIT_FAIL)
			send_message(T_MSG_KO, ERROR_EXEC, sock);
		else
			if (wait_response(sock) == 0)
				send_message(T_MSG_OK, LS_GOOD, sock);
	}
	else if (child == 0)
	{
		send_command(T_LS, "IS OK", sock, 0);
		dup2(sock, STDOUT_FILENO);
		dup2(sock, STDERR_FILENO);
		if (execv("/bin/ls", test) != -1)
		{
			close(sock);
			exit(0);
		}
		else{
			close(sock);
			exit(EXIT_FAILURE);
		}
	}
}

int			get_command(int sock, t_trame trame, t_info *info)
{
	int 		fd;
	off_t		size;

//	printf("DEBUG : get_command => PATH : %s | SIZE : %llu\n", trame.value, trame.size);
	if ((fd = open(trame.value, O_RDWR)) != -1)
	{
		if ((size = get_file_size(fd, sock)) == -1)
			return (RET_KO);
		send_command(T_GET, get_name_from_path(trame.value), sock, size);
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


int				manage_command(int cs, t_trame trame, t_info *info)
{
	char		*value;
	char 		*cmd;

	if (ft_memcmp(trame.type, T_LS, CMD_SIZE) == 0)
	{
		printf("DEBUG : LS COMMAND\n");
		ls_command(cs);
	}
	else if (ft_memcmp(trame.type, T_PWD, CMD_SIZE) == 0)
	{
		printf("DEBUG : PWD COMMAND\n");
		format_path(info, cs, T_PWD);
	}
	else if (ft_memcmp(trame.type, T_CD, CMD_SIZE) == 0)
	{
		printf("DEBUG : CD COMMAND\n");
		cd_command(cs, trame, info);
	}
	else if (ft_memcmp(trame.type, T_GET, CMD_SIZE) == 0)
	{
		printf("DEBUG : GET COMMAND\n");
		return (get_command(cs, trame, info));
	}
	else if (ft_memcmp(trame.type, T_PUT, CMD_SIZE) == 0)
	{
		printf("DEBUG : GET COMMAND\n");
		return (put_command(cs, trame, info));
	}
	else
		send_message(T_MSG_KO, COMMAND_NOT_FOUND, cs);
	return 0;
}