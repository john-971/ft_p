#include "../includes/ft_p.h"

int		parse_command(char *input, int sock)
{
	char 	**commands;

//	printf("PARSE COMMANDS :\n");
	commands = ft_strsplit(input, ' ');
//	ft_read_tab(commands);
	if (ft_strcmp(commands[0], "ls") == 0)
		send_command(T_LS, input, sock);
	else if (ft_strcmp(commands[0], "pwd") == 0)
		send_command(T_PWD, input, sock);
	else if (ft_strcmp(commands[0], "cd") == 0)
	{
		if (commands[1] && ft_strlen(commands[1]) > 0)
			send_command(T_CD, commands[1], sock);
		else
			send_command(T_CD, "/", sock);
	}
	else
	{
		print_error(COMMAND_NOT_FOUND);
		return (1);
	}

	return 0;
}