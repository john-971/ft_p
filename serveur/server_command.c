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

void				ls_command(int sock)
{
	DIR				*dip;
	struct dirent	*dit;
	char 			cwd[PATH_MAX];

	printf("IN LS COMMAND \n");
	if(getcwd(cwd, sizeof(cwd)) == NULL)
		send_message(T_MSG_KO, ERROR_OPEN, sock);
	printf("CWD : %s\n", cwd);
	if ((dip = opendir(cwd)) == NULL)
	{
		send_message(T_MSG_KO, ERROR_OPEN, sock);
		return;
	}
	while ((dit = readdir(dip)) != NULL)
	{
		printf("DIR NAME %s\n", dit->d_name);
	}

	//ICI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! reste a renvoyer les infos (avec la taille ?)

}

int				manage_command(int cs, char *buff)
{
	char		*value;
	char 		*cmd;

//	printf("DEBUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!: %s\n", buff);
	value = buff + CMD_SIZE;
	if (ft_memcmp(buff, T_LS, CMD_SIZE) == 0)
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