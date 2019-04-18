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
	DIR				*dip;
	struct dirent	*dit;
	char 			cwd[PATH_MAX];
	int 			fd;

	printf("IN LS COMMAND \n");
	if(getcwd(cwd, sizeof(cwd)) == NULL)
		send_message(T_MSG_KO, ERROR_OPEN, sock);
	printf("CWD : %s\n", cwd);
	if ((dip = opendir(cwd)) == NULL)
	{
		send_message(T_MSG_KO, ERROR_OPEN, sock);
		return;
	}
	fd = create_file();
	while ((dit = readdir(dip)) != NULL)
	{
		write(fd, dit->d_name, ft_strlen(dit->d_name));
		write(fd, "\n", 1);
		printf("DIR NAME %s\n", dit->d_name);
	}
	close(fd);
	closedir(dip);

	send_message(T_MSG_OK, "IS OK", sock);
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