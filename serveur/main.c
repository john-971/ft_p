#include "../includes/ft_p.h"

//attention au port qui peut etre deja pris
void 					usage(char *str)
{
	printf("usage : %s <port>", str);
	exit(EXIT_FAILURE);
}

int						manage_login(int sock, int max_try)
{
	t_trame				trame;
	char 				*login;
	char 				*password;

	if (max_try == MAX_TRY)
	{
		send_command(T_BYE, T_BYE, sock, 0);
		return -1;
	}
	send_command(T_LOG, V_LOGIN, sock, 0);
	if ((trame = listen_sock(sock)).error == 1)
		return -1;
	login = ft_strdup(trame.value);
	send_command(T_LOG, V_PASS, sock, 0);
	if ((trame = listen_sock(sock)).error == 1)
		return -1;
	password = ft_strdup(trame.value);
	if (ft_strcmp(login, "john") == 0 && ft_strcmp(password, "bobby") == 0)
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
		return manage_login(sock, max_try + 1);
	}
}

void					main_process(int m_sock, uint32_t cslen)
{
	int 				cs;
	t_trame				trame;
	int 				pid;
	t_info				info;
	struct sockaddr_in	csin;

	while ((cs = accept(m_sock, (struct sockaddr*)&csin, &cslen)) != -1)
	{
		pid = fork();
		if(pid == -1)
			exit(EXIT_FAILURE);
		printf("PID %i\n", pid);
		if (pid == 0)
		{
			printf("\033[0;33mClient connected [%i] \033[0m\n", cs);
			if (manage_login(cs, 0) == 0)
			{
				info.base_path = NULL;
				info.path = NULL;
				info.base_path = set_path(info.base_path);
				info.path = set_path(info.path);
//				printf("START PATH ! : %s : lvls %i\n", info.base_path, info.b_path_lvl);
				printf("\033[0;33mClient [%i], logged \033[0m\n", cs);
				while (1)
				{
					printf("DEBUG : ACTUAL PATH %s\n", info.path);
					if ((trame = listen_sock(cs)).error == 1)
						break;
					if (manage_command(cs, trame, &info) == -1)
						break;
				}
			}
			printf("\033[0;33mClose connection for [%i]\033[0m\n", cs);
			close (cs);
		}
	}
}

int 					main(int ac, char **av)
{
	int 				port;
	int 				m_sock;
	uint32_t	 		cslen;

	cslen = 0;
	if (ac != 2)
		usage(av[0]);
	port = atoi(av[1]);
	m_sock = create_server(port);
	if (m_sock != -1)
	{
		main_process(m_sock, cslen);
		printf("END OF PROGRAM \n");
	}
	else
		exit(EXIT_FAILURE);

}