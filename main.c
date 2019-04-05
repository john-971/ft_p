#include "includes/ft_p.h"

//attention au port qui peut etre deja pris
void 					usage(char *str)
{
	printf("tamere %s", str);
	exit(-1);
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
	bind(sock, (const struct sockaddr*)&sin, sizeof(sin));	//lance l'ecoute sur l'adresse et le port
	listen(sock, 42);	// limite le nombre de connexions simultanée
	return(sock);
}


int 					main(int ac, char **av)
{
	int 				pid;
	int 				port;
	int 				m_sock;
	int 				cs;
	uint32_t	 		cslen;
	struct sockaddr_in	csin;
	int					r;
	char				buff[1024];
	int 				test_signal;
	int 				sock;
	int 				count;
	int 				fd;

	count = 0;
	if (ac != 2)
		usage(av[0]);
	port = atoi(av[1]);
	m_sock = create_server(port);
	if (sock != -1)
	{
		while ((cs = accept(m_sock, (struct sockaddr*)&csin, &cslen)) != -1)
		{

				pid = fork();
				printf("PID %i\n", pid);
				if (pid != 0)
				{
					sock = create_server(port);
					if (sock == -1)
					{
						printf("ERROR\n");
						return 0;
					}
//					fd = accept(sock, (struct sockaddr*)&csin, &cslen);
					printf("Client connected %i\n", sock);
					while ((r = read(sock, buff, 1023)) > 0)
					{
						buff[r] = '\0';
						printf("%s", buff);
					}
					printf("Close connection for %i", sock);
					close (cs);
					close(sock);
				}
//				cs = accept(sock, (struct sockaddr*)&csin, &cslen);
		}
		printf("END OF PROGRAM \n");
	}

}