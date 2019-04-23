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
	printf("SIZE VALUE %i\n", size);
	trame.value[size] = '\0';
	send(sock, &trame, sizeof(trame), 0);
}

void		send_command(char *type, char *value, int sock, off_t size)
{
	t_trame trame;
//	printf("DEBUG SEND COMMAND => VALUE : %s\n", value);
	ft_bzero(&trame, sizeof(t_trame));
	ft_memcpy(trame.type, type, CMD_SIZE);
	trame.type[5] = '\0';
	ft_memcpy(trame.value, value, ft_strlen(value));
	trame.value[TRANS_SIZE] = '\0';

	trame.size = size;
//	printf("DEBUG : LISTEN => VALUE : %s | TYPE : %s | LEN %zu\n", trame.value, trame.type, ft_strlen(trame.value));
	send(sock, &trame, sizeof(trame), 0);
}

t_trame					listen_sock(int sock)
{
	int					r;
	char				*buff[sizeof(t_trame) + 1];
	t_trame				*trame;

//	ft_bzero(buff, sizeof(t_trame) + 1);
//	printf("LISTEN SOCK !!! \n");
	r = recv(sock, buff, sizeof(t_trame), 0);
	buff[r] = "\0";
	trame = (t_trame *)buff;
	if (r <= 0)
	{
		printf("ERROR ON LISTEN SOCK\n");
		trame->error = 1;
	}
	trame->read = r;

//	printf("DEBUG : LISTEN => VALUE : %s | TYPE : %s\n", trame->value, trame->type);
	return *trame;
}

int						wait_response(int sock)
{
	t_trame				trame;

	trame = listen_sock(sock);
	if (trame.error == 1)
		return (1);
	if (ft_strcmp(trame.value, OK) == 0)
		return (0);
	return (1);
}


