#include "../includes/ft_p.h"


int				manage_put(int sock, char *file_path)
{
	int 		fd;
	off_t		size;
	t_trame		trame;

//	printf("DEBUG : get_command => PATH : %s | SIZE : %llu\n", trame.value, trame.size);

	if ((fd = open(file_path, O_RDWR)) != -1)
	{
		if ((size = get_file_size(fd, sock)) == -1)
			return (1);
		send_command(T_PUT, get_name_from_path(file_path), sock, size);
		trame = listen_sock(sock);
		if (trame.error == -1)
		{
			close(fd);
			return (1);
		}
		if (ft_strcmp(trame.value, OK) == 0)
		{
			send_file(fd, sock, size, T_PUT);
		}
		else{
			printf("DEBUG : GET COMMAND CLIENT ABORT\n");
		}
		close(fd);
	}
	else
	{
		printf("OPEN FAIL \n");
		print_error(get_error());
		return (1);
	}
	return (0);
}

void	free_tab(char **tab)
{
	int j;
	while (tab[j])
	{
		print_succes("IN FREE TAB");
		free(tab[j]);
		j++;
	}
	free(tab);
}

int		parse_command(char *input, int sock)
{
	char 	**commands;
	int 	ret;

	ret = 0;
//	printf("PARSE COMMANDS :\n");
	commands = ft_strsplit(input, ' ');
	if (ft_strcmp(commands[0], "ls") == 0)
		send_command(T_LS, input, sock, 0);
	else if (ft_strcmp(commands[0], "pwd") == 0)
		send_command(T_PWD, input, sock, 0);
	else if (ft_strcmp(commands[0], "cd") == 0)
	{
		if (commands[1] && ft_strlen(commands[1]) > 0)
			send_command(T_CD, commands[1], sock, 0);
		else
			send_command(T_CD, "/", sock, 0);
	}
	else if (ft_strcmp(commands[0], "get") == 0)
	{
		if (commands[1] && ft_strlen(commands[1]) > 0)
			send_command(T_GET, commands[1], sock, 0);
		else
		{
			print_error(PARAM_MISSING);
			ret = 1;
		}
	}
	else if (ft_strcmp(commands[0], "put") == 0)
	{
		if (commands[1] && ft_strlen(commands[1]) > 0)
			ret = manage_put(sock, commands[1]);
		else
		{
			print_error(PARAM_MISSING);
			ret = 1;
		}
	}
	else
	{
		print_error(COMMAND_NOT_FOUND);
		ret = 1;
	}
	free_tab(commands);
	return (ret);
}