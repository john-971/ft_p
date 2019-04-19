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
	send_command(T_LS, "IS OK", sock);
	if ((child = fork()) > 0)
	{
		if(wait4(child, &exit_status, 0, NULL) == -1)
			send_message(T_MSG_KO, ERROR_EXEC, sock);
//		else
//			send_message(T_MSG_OK, "IS OK", sock);
	}
	else if (child == 0)
	{
		dup2(sock, STDOUT_FILENO);
		dup2(sock, STDERR_FILENO);
		if (execv("/bin/ls", test) != -1)
		{
			close(sock);
			exit(1);
		}
		else{
			close(sock);
			exit(-1);
		}
	}
}

void				pwd_command(int sock)
{
	char 			cwd[PATH_MAX];

	if(getcwd(cwd, sizeof(cwd)) == NULL)
		send_message(T_MSG_KO, get_error(), sock);
	else
		send_command(T_PWD, cwd, sock);
//	printf("PWD PATH : %s\n", cwd);
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
		pwd_command(cs);
	}
	else if (ft_memcmp(trame.type, T_CD, CMD_SIZE) == 0)
	{
		printf("DEBUG : CD COMMAND\n");
		cd_command(cs, trame, info);
//		printf("DEBUG : END CD \n");
	}
	else
		send_message(T_MSG_KO, COMMAND_NOT_FOUND, cs);
	return 0;
}