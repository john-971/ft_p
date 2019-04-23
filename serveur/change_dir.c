#include "../includes/ft_p.h"

/**
** Gestion de la commande cd pour se deplacer sur le serveur
*/

char					*set_path(char *path)
{
	char 				cwd[TRANS_SIZE];

	if(getcwd(cwd, TRANS_SIZE) == NULL)
	{
		print_error(get_error());
		exit(-1);
	}
	if (path)
		free(path);
	path = ft_strdup(cwd);
	print_succes(path);
	return path;
}

void				format_path(t_info *info, int sock, char *type)
{
	char 			*to_send;
	int 			i;

	i = 0;
	while (info->path[i] && info->base_path[i])
	{
		if (info->path[i] == info->base_path[i])
			i++;
		else
			break;
	}
	if (!info->path[i] && !info->base_path[i])
		send_command(type, "/", sock, 0);
	else
	{
		to_send = ft_strsub(info->path, i, ft_strlen(info->path) - i);
		send_command(type, to_send, sock, 0);
	}
	if(type == T_PWD)
		send_message(T_MSG_OK, PWD_GOOD, sock);
	else if(type == T_CD)
		send_message(T_MSG_OK, CWD_OK, sock);
}

void				cd_command(int sock, t_trame trame, t_info *info)
{
	char 			*path;

	path = NULL;
	if (chdir(trame.value) == 0)
	{
		path = set_path(path);
//		printf("STRSTR %s <=> %s\n", path, info->base_path);
		if (ft_strstr(path, info->base_path) == NULL || trame.value[0] == '/')
		{
//			printf("PATH NOT OK \n");
			if (chdir(info->base_path) == 0)
			{
				info->path = set_path(info->path);
				format_path(info, sock, T_CD);
			}
			else
				send_message(T_MSG_KO, get_error(), sock);
		}
		else
		{
//			printf("PATH OK \n");
			info->path = set_path(info->path);
			format_path(info, sock, T_CD);
		}
		free(path);
	}
	else
		send_message(T_MSG_KO, get_error(), sock);
}