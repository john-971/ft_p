
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
#include <errno.h>


#define TRANS_SIZE 1023
#define RET_KO 0
#define RET_OK 1

#define T_MSG	"<MSG>"
#define T_MSG_OK 0
#define T_MSG_KO 1

#define T_LOG 	"<LOG>"
#define	T_CMD 	"<CMD>"
#define T_RET 	"<RET>"
#define	T_END 	"<END>"

#define T_LS  	"<LIS>"
#define T_CD 	"<CWD>"
#define T_PWD 	"<PWD>"

//USHRT_MAX pour le transfert de fichiers !

typedef struct 	s_trame
{
	char		type[6];
	uint8_t 	type_msg;
	char 		value[1024];
	int			error;
}				t_trame;

typedef struct 	s_info
{
	char		base_path[PATH_MAX];
	int			b_path_lvl;
	char		path[PATH_MAX];
}				t_info;


#define CMD_SIZE 5

#define V_LOGIN "Login"
#define V_PASS "Password"
#define BAD_LOG "Informations de connexions non reconnue"
#define GOOD_LOG "Login ok !"
#define PARAM_MISSING "Il manque un param pour la commande"
#define ERROR_OPEN "Une erreur est survenu dans l'ouverture du dossier"
#define ERROR_EXEC "Une erreur est survenu dans l'execution de la commande"
#define COMMAND_NOT_FOUND "Commande inconnue (ls/cd/pwd/get/put)"

#define ERR_NOTDIR "Un élément du chemin d'accès n'est pas un répertoire"
#define ERR_ACL "Impossible de lire ou de parcourir un composant du chemin"
#define ERR_ENOENT "Le répertoire en cours a été supprimé."
#define CWD_OK "Changement du repertoire de travail !"
#define ERROR_DEFAULT "Une erreur est surevenu"

#include "../libft/includes/libft.h"


/**
 **====================== SERVER SIDE
**/


/**
 ** server_command.c
**/
int						manage_command(int cs, t_trame trame, t_info *info);

/**
 ** change_dir.c
**/
void					cd_command(int sock, t_trame trame, t_info *info);

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
void					init_path(t_info *info, int sock);

/**
 ** ===================== COMMON
**/

/**
 ** send_receive.c
**/
void					send_message(uint8_t type_message, char *value, int sock);
void					send_command(char *type, char *value, int sock);
t_trame					listen_sock(int sock);

/**
 ** create_socket.c
**/
int 					create_server(int port);
int 					create_client(char *addr, int port);

/**
 ** manage_errno.c
**/
char					*get_error();
#endif
