#include "../includes/ft_p.h"

//attention au port qui peut etre deja pris
void 					usage(char *str)
{
	printf("usage : %s <port>", str);
	exit(EXIT_FAILURE);
}

int						manage_login(int sock)
{
	t_trame				trame;
	char 				*login;
	char 				*password;

	send_command(T_LOG, V_LOGIN, sock);
	if ((trame = listen_sock(sock)).error == 1)
		return -1;
	login = ft_strdup(trame.value);
	send_command(T_LOG, V_PASS, sock);
	if ((trame = listen_sock(sock)).error == 1)
		return -1;
	password = ft_strdup(trame.value);
	if (ft_strstr(login, "john") != NULL && ft_strstr(password, "bobby") != NULL)
	{
		send_message(T_MSG_OK, GOOD_LOG, sock);
		free(login);
		free(password);
		return 0;
	}
	else
	{
		free(login);
		free(password);
		send_message(T_MSG_KO, BAD_LOG, sock);
		return manage_login(sock);
	}
}

void					main_process(int m_sock, uint32_t cslen, struct sockaddr_in csin)
{
	int 				cs;
	t_trame				trame;
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
			printf("Client connected %i\n", cs);
			if (manage_login(cs) == 0)
			{
				init_path(&info, cs);
//				printf("START PATH ! : %s : lvls %i\n", info.base_path, info.b_path_lvl);
				printf("Client %i, logged\n", cs);
				while (1)
				{
//					printf("DEBUG : ACTUAL PATH %s", info.path);
					if ((trame = listen_sock(cs)).error == 1)
						break ;

					if (manage_command(cs, trame, &info) == -1)
						break ;
//					printf("DEBUG END COMMAND \n");
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