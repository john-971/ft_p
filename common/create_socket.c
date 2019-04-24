
#include "../includes/ft_p.h"

int 					create_server(int port)
{
	int 				sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;
	int 				sendbuff = FILE_SIZE;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return -1;
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("ERROR\n");
		return -1;
	}
//	int i = 1;
//	res = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuff, sizeof(sendbuff));
	int res = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuff, sizeof(sendbuff));
	printf("RETROUR SETSOCKOPT : %i\n", res);
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

int 					create_client(char *addr, int port)
{
	int 				sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;
	int 				sendbuff = FILE_SIZE;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return -1;
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("ERROR\n");
		return -1;
	}

	int res = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuff, sizeof(sendbuff));
	printf("RETROUR SETSOCKOPT : %i\n", res);
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