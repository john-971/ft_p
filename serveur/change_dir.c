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

	go_to_lvl = count_dir_level(go_to);
	path_lvl = count_dir_level(info->path);
	if ((path_lvl - go_to_lvl) <= info->b_path_lvl)
	{
//		printf("DEBUG : ON VEUX REVENIR TROP HAUT path lvl : %i, goto : %i, b_path_lvl : %i\n", path_lvl, go_to_lvl, info->b_path_lvl);
		return (info->base_path);
	}
	else
	{
//		printf("DEBUG : ON VEUX REVENIR MAIS C OK\n");
		return (go_to);
	}
}

void				format_path(t_info *info, int sock)
{
	char 			*to_send;
	int 			i;

	i = 0;
	while (info->path[i] && info->base_path[i])
	{
//		printf("COMPAAAAARE %c : %c\n", info.path[i], info.base_path[i]);
		if (info->path[i] == info->base_path[i])
			i++;
		else
			break;
	}

	if (!info->path[i] && !info->base_path[i])
		send_command(T_CD, "/", sock);
	else
	{
		to_send = ft_strsub(info->path, i, ft_strlen(info->path) - i);
//		printf("TOSEEEEEEEEEEEND %s\n", to_send);
		send_command(T_CD, to_send, sock);
	}
}

void				cd_command(int sock, t_trame trame, t_info *info)
{
	char 			cwd[PATH_MAX];
	char 			*go_to;

	if (ft_strstr(trame.value, "..") || ft_strstr(trame.value, "..") || trame.value[0] == '/')
	{
//		printf("DEBUG : FIND GO BACK IN CD\n");
		go_to = manage_go_back(info, trame.value);
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
//		printf("CWD !!!!! %s\n", cwd);
		ft_bzero(info->path, sizeof(info->path));
		ft_memcpy(&info->path, cwd, ft_strlen(cwd));
//		printf("NEW PATH %s\n", info.path);
		format_path(info, sock);
	}
	else
		send_message(T_MSG_KO, get_error(), sock);
}