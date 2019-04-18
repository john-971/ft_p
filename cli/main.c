#include "../includes/ft_p.h"



//attention au port qui peut etre deja pris
void 					usage(char *str)
{
	printf("usage : %s <address> <port>", str);
	exit(EXIT_FAILURE);
}

void					handle_connexion(int sig)
{
	printf("!!!!!!!!!!!!!!!!!!!!! %i\n", sig);
//	return 1;
}

int 					create_client(char *addr, int port)
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
	sin.sin_addr.s_addr = *((size_t *)(gethostbyname(addr))->h_addr_list[0]);
	if (connect(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		printf("Connect error");
		exit(2);
	}
	listen(sock, 2);	// limite le nombre de connexions simultanée
	return(sock);
}

//int						manage_ret(char *response)
//{
//	if (ft_memcpy(msg, T_CMD, CMD_SIZE))
//	{}
//}

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
		send_message(T_LOG, u_input, sock);
	else
		manage_login(sock, msg_value);
}

int					parse_msg(char *msg, int sock)
{
	char				*value;

	value = msg + CMD_SIZE;
	if (ft_memcmp(msg, T_LOG, CMD_SIZE) == 0)
	{
//		printf("DEBUG : LOGIN COMMAND : %s\n", value);
		manage_login(sock, value);
		return (1);
	}
	else if (ft_memcmp(msg, T_MSG_KO, CMD_SIZE) == 0)
	{
		/*
		 * EN CAS DE MESSAGE D'ERREUR AUTRE QUE LE LOGIN, IL FAUT RENDRE LE PROMPT
		 */
		value = ft_strsub(value, 0, ft_strlen(value) - CMD_SIZE);
		print_error(value);
		free(value);
		return (1);
	}
	else if (ft_memcmp(msg, T_MSG_OK, CMD_SIZE) == 0)
	{
		value = ft_strsub(value, 0, ft_strlen(value) - CMD_SIZE);
		print_succes(value);
		free(value);
		return (0);
	}
	else{
		printf("DEBUG : TYPE UNKNOW\n");
	}
	return (0);
}

void					prompt(int sock)
{
	char 				*u_input;
	char 				*cmd;

	ft_putstr("ft_p:>");
	get_next_line(0, &u_input);
	if (ft_strlen(u_input) > 0)
	{
		if (parse_command(u_input, sock) == 0)
			return ;
		else
			prompt(sock);
	}
	prompt(sock);


}

void					main_process(int m_sock, uint32_t cslen, struct sockaddr_in csin)
{

	char 				s_input[TRANS_SIZE + 1];
	int 				r;

	while (1)
	{
//		r = recv(m_sock, s_input, 1023, 0);
		if (listen_sock(m_sock, s_input) == -1)
			break;
//		s_input[r] = '\0';
		printf("DEBUG : %s\n", s_input);
		if (ft_strstr(s_input + (ft_strlen(s_input) - CMD_SIZE), T_END) != NULL)
		{
			if (parse_msg(s_input, m_sock) == 0)
			{
				prompt(m_sock);
//				ft_putstr("ft_p:>");
//				get_next_line(0, &u_input);
//				if (ft_strlen(u_input) > 0)
//					send_message(T_CMD, u_input, m_sock);
//				else
//					send_message(T_CMD, "TAMERE", m_sock); //PAS BEAU
			}
			printf("DEBUG : CORRECT TRANSMISSION\n");
		}
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