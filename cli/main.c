#include "../includes/ft_p.h"



//attention au port qui peut etre deja pris
void 					usage(char *str)
{
	printf("usage : %s <address> <port>", str);
	exit(EXIT_FAILURE);
}


void					manage_login(int sock, char *msg_value)
{
	char 				*u_input;
	int					r;

	if (ft_memcmp(msg_value, V_LOGIN, ft_strlen(V_LOGIN)) == 0)
		ft_putstr("Login :");
	else if (ft_memcmp(msg_value, V_PASS, ft_strlen(V_PASS)) == 0)
		ft_putstr("Password :");
	get_next_line(0, &u_input);
	if (ft_strlen(u_input) > 0)
		send_command(T_LOG, u_input, sock);
	else
		manage_login(sock, msg_value);
}

void					manage_ls(int sock)
{
	char				buff[TRANS_SIZE + 1];
	int					r;
	t_trame				*trame;

	while (1)
	{
		r = recv(sock, buff, TRANS_SIZE, 0);
		if (r == -1)
		{
			print_error(ERROR_EXEC);
			return ;
		}
		buff[r] = '\0';
		trame = (t_trame *)buff;
		if (trame->type == T_MSG)
		{
			print_error(trame->value);
			return ;
		}
		else
			printf("%s", buff);
		if(r < TRANS_SIZE)
			break;
	}
}

void				manage_pwd(t_trame trame, int sock)
{
	printf("%s\n", trame.value);
}

int					parse_msg(t_trame trame, int sock, t_info *info)
{
	char				*value;

//	printf("DEBUG PARSE MESSAGE :type : %s | value : %s\n", trame.type, trame.value);
	if (ft_memcmp(trame.type, T_LOG, CMD_SIZE) == 0)
	{
		manage_login(sock, trame.value);
		return (1);
	}
	else if (ft_memcmp(trame.type, T_LS, CMD_SIZE) == 0)
	{
		manage_ls(sock);
		return (1);
	}
	else if (ft_memcmp(trame.type, T_PWD, CMD_SIZE) == 0)
	{
		manage_pwd(trame, sock);
		return (1);
	}
	else if (ft_memcmp(trame.type, T_CD, CMD_SIZE) == 0)
	{
		ft_bzero(info->path, sizeof(info->path));
		ft_memcpy(info->path, trame.value, ft_strlen(trame.value));
		return (1);
	}
	else if (ft_memcmp(trame.type, T_MSG, CMD_SIZE) == 0)
	{
		if (trame.type_msg == T_MSG_KO)
		{
			print_error(trame.value);
			if (ft_memcmp(trame.value, BAD_LOG, ft_strlen(BAD_LOG)) == 0)
				return (1);
		}
		else
			print_succes(trame.value);
	}
	else{
		printf("DEBUG : TYPE UNKNOW\n");
	}
	return (0);
}

void					prompt(int sock, t_info	info)
{
	char 				*u_input;
	char 				*cmd;

	print_prompt(info.path);
	get_next_line(0, &u_input);
	if (ft_strlen(u_input) > 0)
	{
		if (parse_command(u_input, sock) == 0)
			return ;
	}
	prompt(sock, info);
}

void					main_process(int m_sock, uint32_t cslen, struct sockaddr_in csin)
{

	t_trame				trame;
	int 				r;
	t_info				info;

	ft_bzero(&trame, sizeof(t_trame));
	ft_bzero(&info, sizeof(t_info));
	info.path[0] = '/';
	while (1)
	{
		if ((trame = listen_sock(m_sock)).error == 1)
			break;
//		printf("DEBUG : %s\n", trame.type);
		if (parse_msg(trame, m_sock, &info) == 0)
		{
			prompt(m_sock, info);
		}
//		printf("DEBUG : CORRECT TRANSMISSION\n");
	}
	close(m_sock);
}

int 					main(int ac, char **av)
{
	int 				port;
	int 				m_sock;
	uint32_t	 		cslen;
	struct sockaddr_in	csin;

	if (ac != 3)
		usage(av[0]);
	port = atoi(av[2]);
	m_sock = create_client(av[1], port);
	if (m_sock != -1)
	{
		printf("Connected to %s\n", av[1]);
		main_process(m_sock, cslen, csin);
		printf("END OF PROGRAM \n");
	}
	else
		exit(EXIT_FAILURE);

}