
#ifndef FT_P_H
#define FT_P_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>


#define TRANS_SIZE 1023
#define RET_KO 0
#define RET_OK 1

#define T_MSG_OK "<MS0>"
#define T_MSG_KO "<MS1>"

#define T_LOG 	"<LOG>"
#define	T_CMD 	"<CMD>"
#define T_RET 	"<RET>"
#define	T_END 	"<END>"

#define T_LS  	"<LIS>"
#define T_CD 	"<CWD>"
#define T_PWD 	"<PWD>"


#define CMD_SIZE 5

#define V_LOGIN "Login"
#define V_PASS "Password"
#define BAD_LOG "Informations de connexions non reconnue"
#define GOOD_LOG "Login ok !"
#define PARAM_MISSING "Il manque un param pour la commande"
#define ERROR_OPEN "Une erreur est survenu dans l'ouverture du dossier"



#include "../libft/includes/libft.h"


/**
 **====================== SERVER SIDE
**/

/**
 ** parse_message.c
**/
void					send_message(char *type, char *value, int sock);
int						listen_sock(int sock, char *buff);

/**
 ** server_command.c
**/
int						manage_command(int cs, char *buff);



/**
 **====================== CLIENT SIDE
**/


/**
 ** output.c
**/
void					print_error(char *msg);
void					print_succes(char *msg);

/**
 ** cli_command.c
**/
int						parse_command(char *input, int sock);


#endif
