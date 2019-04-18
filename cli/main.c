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
		send_command(T_LOG, u_input, sock);
	else
		manage_login(sock, msg_value);
}

int					parse_msg(t_trame trame, int sock)
{
	char				*value;


	if (ft_memcmp(trame.type, T_LOG, CMD_SIZE) == 0)
	{
//		printf("DEBUG : LOGIN COMMAND : %s\n", value);
		manage_login(sock, trame.value);
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

	t_trame				trame;
	int 				r;

	ft_bzero(&trame, sizeof(t_trame));
	while (1)
	{
		if ((trame = listen_sock(m_sock)).error == 1)
			break;
		printf("DEBUG : %s\n", trame.type);
		if (parse_msg(trame, m_sock) == 0)
		{
			prompt(m_sock);
		}
		printf("DEBUG : CORRECT TRANSMISSION\n");

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