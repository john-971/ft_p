#include "../includes/ft_p.h"


void		send_message(char *type, char *value, int sock)
{
	int		size;
	char 	*trame;


	size = ft_strlen(value);
	trame = (char *)ft_memalloc((CMD_SIZE * 2) + size + 1);
	ft_memcpy(trame, type, CMD_SIZE);
	ft_memcpy(trame + CMD_SIZE, value, size);
	ft_memcpy(trame + CMD_SIZE + size, T_END, CMD_SIZE);
	trame[size + (CMD_SIZE * 2) + 1] = '\0';

//	printf("TRAME : %s\n", trame);
	send(sock, trame, size + (CMD_SIZE * 2), 0);
	free(trame);
}


int					listen_sock(int sock, char *buff)
{
	int					r;
	printf("ON LISTEN !!!!\n");
	r = recv(sock, buff, TRANS_SIZE, 0);
	buff[r] = '\0';
//	printf("DEBUG LISTEN SOCK %s \n", buff);
	if (ft_strstr(buff + (r - CMD_SIZE), T_END) != NULL)
	{
//		printf("CORRECT TRANSMISSION\n");
		return (0);
	}
	else
	{
		printf("BAD TRANSMISSION !!!!!!!\n");
		return (-1);
	}
}