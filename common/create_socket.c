
#include "../includes/ft_p.h"

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
		exit(EXIT_FAILURE);
	}
	listen(sock, 2);	// limite le nombre de connexions simultanée
	return(sock);
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
		print_error("Erreur dans la création de la socket");
		return -1;
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = *((size_t *)(gethostbyname(addr))->h_addr_list[0]);
	if (connect(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		print_error("Erreur dans la connection a la socket");
		exit(EXIT_FAILURE);
	}
	return(sock);
}