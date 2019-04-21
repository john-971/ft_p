#include "../includes/ft_p.h"

/**
** Gestion de la commande cd pour se deplacer sur le serveur
*/

int						count_dir_level(char *path)
{
	int 				counter;

	counter = 0;
	if (path[0] == '.' && path[1] == '.' && !path[2])
		return 1;
	while(*path != '\0')
	{
		if (*path == '/')
			counter++;
		path++;
	}
	return counter;
}

void					init_path(t_info *info, int sock)
{
	char 				start_dir[PATH_MAX];

	if(getcwd(start_dir, sizeof(start_dir)) == NULL)
	{
		send_message(T_MSG_KO, get_error(), sock);
		exit(-1);
	}
	ft_memcpy(info->base_path, start_dir, ft_strlen(start_dir));
	ft_memcpy(info->path, start_dir, ft_strlen(start_dir));
	info->b_path_lvl = count_dir_level(start_dir);
}

char				*manage_go_back(t_info *info, char *go_to)
{
	int 			go_to_lvl;
	int 			path_lvl;
	int 			i;

	i = 0;
	if (go_to[0] == '/' && go_to[1] != '\0')
	{
		while (go_to[i] && go_to[++i] != '/')
		{
			if (go_to[i] != info->base_path[i])
				return NULL;
		}
	}
	go_to_lvl = count_dir_level(go_to);
	path_lvl = count_dir_level(info->path);
	if ((path_lvl - go_to_lvl) <= info->b_path_lvl)
		return (info->base_path);
	else
		return (go_to);
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
	char 			cwd[PATH_MAX];
	char 			*go_to;

	if (ft_strstr(trame.value, "..") || ft_strstr(trame.value, "..") || trame.value[0] == '/')
	{
//		printf("DEBUG : FIND GO BACK IN CD\n");
		go_to = manage_go_back(info, trame.value);
		if (go_to == NULL)
		{
			send_message(T_MSG_KO, ERR_ENOENT, sock);
			return;
		}
	}
	else
		go_to = trame.value;
	if (chdir(go_to) == 0)
	{

		if(getcwd(cwd, sizeof(cwd)) == NULL)
		{
			send_message(T_MSG_KO, get_error(), sock);
			return ;
		}
		ft_bzero(info->path, sizeof(info->path));
		ft_memcpy(&info->path, cwd, ft_strlen(cwd));
//		printf("DEBUG : NEW PATH %s\n", info.path);
		format_path(info, sock, T_CD);
	}
	else
		send_message(T_MSG_KO, get_error(), sock);
}