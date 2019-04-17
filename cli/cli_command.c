#include "../includes/ft_p.h"

int		parse_command(char *input, int sock)
{
	char 	**commands;

//	printf("PARSE COMMANDS :\n");
	commands = ft_strsplit(input, ' ');
//	ft_read_tab(commands);
	if (ft_memcmp(commands[0], "ls", 2) == 0)
		send_message(T_LS, input, sock);
	else if (ft_memcmp(commands[0], "pwd", 3))
		send_message(T_PWD, input, sock);
	else if (ft_memcmp(commands[0], "cd", 2) == 0)
	{
		if (commands[1] && ft_strlen(commands[1]) > 0)
			send_message(T_CD, input, sock);
		else
		{
			print_error(PARAM_MISSING);
			return -1;
		}
	}
	return 0;
}