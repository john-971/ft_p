#include "../includes/ft_p.h"



int				find_char_at(char *str, int tofind)
{
	int			i;

	i = 0;
	while (str[i])
	{
		if (str[i] == tofind)
			return i;
		i++;
	}
	return ft_strlen(str);
}

int					create_file()
{
	int 			fd;

	fd = open("/tmp/ls", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd == -1)
		printf("ERREUR DANS LA CREATION DU FICHIER TMP\n");
	return fd;
}

void				ls_command(int sock)
{
	char 			cwd[PATH_MAX];
	char 			*test[3];
	int 			child;
	int 			exit_status;

	exit_status = -1;
	test[0] = "ls";
	test[1] = "-lR";
	test[2] = NULL;
	send_command(T_LS, "IS OK", sock);
	if ((child = fork()) > 0)
	{
		if(wait4(child, &exit_status, 0, NULL) == -1)
			send_command(T_LS, ERROR_EXEC, sock);
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

int				manage_command(int cs, t_trame trame)
{
	char		*value;
	char 		*cmd;

	if (ft_memcmp(trame.type, T_LS, CMD_SIZE) == 0)
	{
		printf("DEBUG : LS COMMAND\n");
		ls_command(cs);


//		value = ft_strsub(value, 0, ft_strlen(value) - CMD_SIZE);
////		printf("DEBUG : VALUE : %s\n", value);
//		cmd = ft_strsub(value, 0, find_char_at(value, ' '));
//		if (ft_strequ(cmd, "ls") == 1)
//		{
//			printf("DEBUG : COMMAND LS\n");
//
//		}
//
//		free(value);
	}
	return 0;
}