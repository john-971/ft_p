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
		ft_putendl_fd(strerror(errno), 2);
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

void					manage_login(int sock)
{
	char 				*u_input;
	int					r;


		get_next_line(0, &u_input);
		if (ft_strlen(u_input) > 1)
		{
			send_message(T_CMD, u_input, sock);
		}

}

void					parse_msg(char *msg, int sock)
{
	char				*value;

	if (ft_memcmp(msg, T_CMD, CMD_SIZE) == 0)
	{
		value = msg + CMD_SIZE;
		printf("DEBUG : IS A COMMAND : %s\n", value);
		if (ft_memcmp(value, V_LOGIN, ft_strlen(V_LOGIN)) == 0)
		{
			printf("DEBUG : LOGIN COMMAND\n");
			ft_putstr("Login :");
			manage_login(sock);
		}
		else if (ft_memcmp(value, V_PASS, ft_strlen(V_PASS)) == 0)
		{
			printf("DEBUG : PASSWORD COMMAND\n");
			ft_putstr("Password :");
			manage_login(sock);
		}
		else{
			printf("DEBUG : COMMAND UNKNOW\n");
		}
	}
	else{
		printf("DEBUG : TYPE UNKNOW\n");
	}
}

void					main_process(int m_sock, uint32_t cslen, struct sockaddr_in csin)
{

	char 				s_input[1024];
	char 				*u_input;
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
			parse_msg(s_input, m_sock);
			printf("DEBUG : CORRECT TRANSMISSION\n");
//			break;
		}


//		ft_putstr("ft_p:>");
//		get_next_line(0, &u_input);
//		write(m_sock, u_input, ft_strlen(u_input));
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