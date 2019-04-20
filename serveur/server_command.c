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
		send_command(T_LS, "IS OK", sock);
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
	struct stat *buff;

	if (fstat(fd, buff) == 0)
	{
		if (buff && buff->st_size)
			return buff->st_size
	}
	send_message(T_MSG_KO, get_error(), sock);
	return -1;
}

void			get_command(int sock, t_trame trame, t_info *info)
{
	int 		fd;
	char 		*buff;
	int 		r;
	off_t		size;

	if (trame.value && ft_strlen(trame.value) > 0)
	{
		if ((fd = open(trame.value, O_RDONLY)) != -1)
		{
			if ((size = get_file_size(fd, sock)) == -1)
				return ;
			send_command(T_GET, size, sock);
			while ((r = read(fd, buff, TRANS_SIZE)) > 0)
			{
				printf("DEBUG : read size => %i", r);
			}
		}
		else
			send_message(T_MSG_KO, get_error(), sock);
	}
	else
		send_message(T_MSG_KO, PARAM_MISSING, sock);

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