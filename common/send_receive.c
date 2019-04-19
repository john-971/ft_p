#include "../includes/ft_p.h"


void		send_message(uint8_t type_message, char *value, int sock)
{
	int		size;
	t_trame trame;

	ft_bzero(&trame, sizeof(t_trame));
	size = ft_strlen(value);
	ft_memcpy(trame.type, T_MSG, CMD_SIZE);
	trame.type_msg = type_message;
	trame.type[5] = '\0';
	ft_memcpy(trame.value, value, size);
	trame.value[size + 1] = '\0';
	send(sock, &trame, sizeof(trame), 0);
}

void		send_command(char *type, char *value, int sock)
{
	int		size;
	t_trame trame;
//	printf("DEBUUUUUUG : %s\n", type);
	ft_bzero(&trame, sizeof(t_trame));
	ft_memcpy(trame.type, type, CMD_SIZE);
	trame.type[5] = '\0';
	ft_memcpy(trame.value, value, ft_strlen(value));
	trame.value[TRANS_SIZE] = '\0';

	send(sock, &trame, sizeof(trame), 0);
}


t_trame					listen_sock(int sock)
{
	int					r;
	char				*buff[sizeof(t_trame) + 1];
	t_trame				*trame;

	printf("LISTEN SOCK !!!!\n");
	r = recv(sock, buff, sizeof(t_trame), 0);
	buff[r] = "\0";
//	printf("DEBUG : LISTEN SOCK TRAME\n");
	trame = (t_trame *)buff;
//	printf("DEBUG : LISTEN SOCK TRAME %i\n", r);
	if (r <= 0)
	{
		printf("ERROR ON LISTEN SOCK\n");
		trame->error = 1;
	}
	return *trame;
}


