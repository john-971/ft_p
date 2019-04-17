
#ifndef FT_P_H
#define FT_P_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define RET_KO 0
#define RET_OK 1

#define	T_CMD "<CMD>"
#define T_RET "<RET>"
#define	T_END "<END>"
#define CMD_SIZE 5

#define V_LOGIN "Login"
#define V_PASS "Password"


typedef struct	s_info{
	uint8_t		logged;
}				t_info;



//A SUPPRIMER !
#include <errno.h>
#include <string.h>



#include "../libft/includes/libft.h"



/**
 ** parse_message.c
**/
void				send_message(char *type, char *value, int sock);
int					listen_sock(int sock, char *buff);

#endif
