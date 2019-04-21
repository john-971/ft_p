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
//		wait4(child, &exit_status, 0, NULL);
//		printf("RETOUR WAIT FOR CHILD !!!!!! %i\n", exit_status == 256);
		wait4(child, &exit_status, 0, NULL);
		if(exit_status == RET_EXIT_FAIL)
			send_message(T_MSG_KO, ERROR_EXEC, sock);
		else
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


off_t			get_file_size(int fd, int sock)
{
	off_t		size;
	struct stat buff;


	if (fstat(fd, &buff) == 0)
	{
		printf("DEBUG : GET FILE SIZE => %llu\n", buff.st_size);
//		if (buff.st_size)
			return (buff.st_size);
	}
	send_message(T_MSG_KO, get_error(), sock);
	return (-1);
}

char			*get_name_from_path(char *path)
{
	char 		*name;
	int 		i;
	int 		nb_lvl;

	name = path;
	while((path = ft_memchr(path, '/', ft_strlen(path))) != NULL)
	{
		path++;
		name = path;
//		printf("DEBUG : get_name_from_path => NAME : %s \n", name);
	}
	printf("DEBUG : get_name_from_path => NAME : %s\n", name);
	return name;
}

void			get_command(int sock, t_trame trame, t_info *info)
{
	int 		fd;
	char 		buff[TRANS_SIZE + 1];
	int 		r;
	off_t		size;

	printf("DEBUG : get_command => PATH : %s | SIZE : %llu\n", trame.value, trame.size);

	if ((fd = open(trame.value, O_RDWR)) != -1)
	{
		if ((size = get_file_size(fd, sock)) == -1)
			return ;
		send_command(T_GET, get_name_from_path(trame.value), sock, size);
		trame = listen_sock(sock);
		if (trame.error == -1)
			exit(EXIT_FAILURE);
		if (ft_strcmp(trame.value, OK) == 0)
		{
			printf("DEBUG : GET COMMAND CLIENT SAID OK\n");
			while ((r = read(fd, buff, TRANS_SIZE)) > 0)
			{
				buff[r] = '\0';
				printf("DEBUG : read size => %i \n %s\n", r, buff);
				send_command(T_GET, buff, sock, r);

					trame = listen_sock(sock);
					if (trame.error == -1)
						exit(EXIT_FAILURE);
					if (trame.value == ABORT)
						return ;
			}
			send_message(T_MSG_OK, GET_OK, sock);
//			print_error(get_error());
		}
		else{
			printf("DEBUG : GET COMMAND CLIENT ABORT\n");
		}
	}
	else
		send_message(T_MSG_KO, get_error(), sock);
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
		get_command(cs, trame, info);
	}
	else
		send_message(T_MSG_KO, COMMAND_NOT_FOUND, cs);
	return 0;
}