#include "../includes/ft_p.h"

//attention au port qui peut etre deja pris
void 					usage(char *str)
{
	printf("usage : %s <port>", str);
	exit(EXIT_FAILURE);
}

void					handle_connexion(int sig)
{
	printf("!!!!!!!!!!!!!!!!!!!!! %i\n", sig);
//	return 1;
}

int 					create_server(int port)
{
	int 				sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return -1;
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("ERROR\n");
		ft_putendl_fd(strerror(errno), 2);
		return -1;
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);	// on accepte n'importe quelle adresse
	if (bind(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		printf("Bind error");
		exit(2);
	}
	listen(sock, 2);	// limite le nombre de connexions simultanée
	return(sock);
}

void					manage_login(int sock)
{
	char 				buff[1024];
	int					r;
	char 				*login;
	char 				*password;
	char 				*tmp;

	printf("IN MANAGE LOGIN\n");
	send_message(T_CMD, V_LOGIN, sock);
	listen_sock(sock, buff);
	login = ft_strsub(ft_strchr(buff, '>'), 1, ft_strlen(buff) - (CMD_SIZE * 2));
	printf("LOGIN : %s\n", login);
	send_message(T_CMD, V_PASS, sock);
	listen_sock(sock, buff);
	password = ft_strsub(ft_strchr(buff, '>'), 1, ft_strlen(buff) - (CMD_SIZE * 2));
	printf("PASSWORD : %s\n", password);

	if (ft_strstr(login, "john") != NULL && ft_strstr(password, "bobby") != NULL)
	{
		printf("LOGGED\n");
		return ;
	}
	else
	{
		printf("nu nu nu nu nu nu \n");
		manage_login(sock);
	}

//	while ((r = recv(sock, buff, 1023, 0)) > 0)
//	{
//		buff[r] = '\0';
//		if (ft_strstr(buff + (r - CMD_SIZE), T_END) != NULL)
//		{
//			printf("CORRECT TRANSMISSION\n");
//			login = ft_strsub(ft_strchr(buff, '>'), 1, ft_strlen(buff) - (CMD_SIZE * 2));
//			printf("LOGIN : %s\n", login);
//
//		}
//	}
}

void					main_process(int m_sock, uint32_t cslen, struct sockaddr_in csin)
{
	int 				cs;
	char				buff[1024];
	int					r;
	int 				pid;
	t_info				info;

	while ((cs = accept(m_sock, (struct sockaddr*)&csin, &cslen)) != -1)
	{
		pid = fork();
		if(pid == -1)
			exit(EXIT_FAILURE);
		printf("PID %i\n", pid);
		if (pid == 0)
		{
			manage_login(cs);
			printf("Client connected %i\n", cs);
			while (1)
			{
				if (listen_sock(cs, buff) == -1)
					break ;
				printf("msg receive !!!!\n");
//				r = recv(cs, buff, 1023, 0);
//				if (r == -1)
//					break;
//				buff[r] = '\0';
				printf("%s\n", buff);
				send(cs, "IS OK\n", 6, 0);
			}
			printf("Close connection for %i\n", cs);
			close (cs);
		}
	}
}

int 					main(int ac, char **av)
{
	int 				port;
	int 				m_sock;
	uint32_t	 		cslen;
	struct sockaddr_in	csin;

	if (ac != 2)
		usage(av[0]);
	port = atoi(av[1]);
	m_sock = create_server(port);
	if (m_sock != -1)
	{
		main_process(m_sock, cslen, csin);
		printf("END OF PROGRAM \n");
	}
	else
		exit(EXIT_FAILURE);

}