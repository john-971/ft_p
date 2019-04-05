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

int 					create_serveur(int port)
{
	int 				sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return -1;
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		ft_putendl_fd(strerror(errno), 2);
		return -1;
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sock, (const struct sockaddr*)&sin, sizeof(sin));	//lance l'ecoute sur l'adresse et le port
	listen(sock, 42);	// limite le nombre de connexions simultanée
	return(sock);
}


int 					main(int ac, char **av)
{
	int 				pid;
	int 				port;
	int 				sock;
	int 				cs;
	uint32_t	 		cslen;
	struct sockaddr_in	csin;
	int					r;
	char				buff[1024];
	int 				test_signal;

	if (ac != 2)
		usage(av[0]);
	port = atoi(av[1]);
	sock = create_serveur(port);
	if (sock != -1)
	{

		while (1)
		{
			test_signal = signal(SIGIO, handle_connexion);
			printf("%i\n", test_signal);
			if (test_signal == 0)
			{
				pid = fork();
//				printf("PID %i\n", pid);
				cs = accept(sock, (struct sockaddr*)&csin, &cslen);
//				while ((r = read(cs, buff, 1023)) > 0)
//				{
//					buff[r] = '\0';
//					printf("%s", buff);
//				}
//				close (cs);
//				close(sock);
			}

		}
	}

}