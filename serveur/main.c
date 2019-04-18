#include "../includes/ft_p.h"

//attention au port qui peut etre deja pris
void 					usage(char *str)
{
	printf("usage : %s <port>", str);
	exit(EXIT_FAILURE);
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

int						manage_login(int sock)
{
	char 				buff[TRANS_SIZE + 1];
	char 				*login;
	char 				*password;

	send_message(T_LOG, V_LOGIN, sock);
	if (listen_sock(sock, buff) == -1)
		return -1;
	login = ft_strsub(ft_strchr(buff, '>'), 1, ft_strlen(buff) - (CMD_SIZE * 2));
	send_message(T_LOG, V_PASS, sock);
	if (listen_sock(sock, buff) == -1)
		return -1;
	password = ft_strsub(ft_strchr(buff, '>'), 1, ft_strlen(buff) - (CMD_SIZE * 2));
	if (ft_strstr(login, "john") != NULL && ft_strstr(password, "bobby") != NULL)
	{
		send_message(T_MSG_OK, GOOD_LOG, sock);
		return 0;
	}
	else
	{
		send_message(T_MSG_KO, BAD_LOG, sock);
		return manage_login(sock);
	}
}

void					main_process(int m_sock, uint32_t cslen, struct sockaddr_in csin)
{
	int 				cs;
	char				buff[TRANS_SIZE + 1];
	int					r;
	int 				pid;

	while ((cs = accept(m_sock, (struct sockaddr*)&csin, &cslen)) != -1)
	{
		pid = fork();
		if(pid == -1)
			exit(EXIT_FAILURE);
		printf("PID %i\n", pid);
		if (pid == 0)
		{
			if (manage_login(cs) == 0)
			{
				printf("Client connected %i\n", cs);
				while (1)
				{
					if (listen_sock(cs, buff) == -1)
						break ;
					char *test = ft_strdup(buff);

					if (manage_command(cs, test) == -1)
						break ;
//					printf("%s\n", test);
//					send_message(T_MSG_OK, "IS OK", cs);
				}
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