/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 16:49:11 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/30 16:49:15 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_p.h"

int				manage_put(int sock, char *file_path)
{
	int			fd;
	off_t		size;
	t_trame		trame;
	int			ret;

	ret = 0;
	if ((fd = open(file_path, O_RDWR)) != -1)
	{
		if ((size = get_file_size(fd, sock)) == -1)
			return (1);
		send_command(T_PUT, get_name_from_path(file_path), sock, size);
		trame = listen_sock(sock);
		if (trame.error == -1)
			ret = 1;
		else if (ft_strcmp(trame.value, OK) == 0)
			send_file(fd, sock, size, T_PUT);
		close(fd);
	}
	else
	{
		print_error(get_error());
		ret = 1;
	}
	return (ret);
}

void			free_tab(char **tab)
{
	int j;

	j = 0;
	while (tab && tab[j])
	{
		free(tab[j]);
		j++;
	}
	free(tab);
}

int				incomplete_param(void)
{
	print_error(PARAM_MISSING);
	return (1);
}

int				parse_command2(char **commands, int sock)
{
	if (ft_strcmp(commands[0], "get") == 0)
	{
		if (commands[1] && ft_strlen(commands[1]) > 0)
			send_command(T_GET, commands[1], sock, 0);
		else
			return (incomplete_param());
	}
	else if (ft_strcmp(commands[0], "put") == 0)
	{
		if (commands[1] && ft_strlen(commands[1]) > 0)
			return (manage_put(sock, commands[1]));
		else
			return (incomplete_param());
	}
	else
	{
		print_error(COMMAND_NOT_FOUND);
		return (1);
	}
	return (0);
}

int				parse_command(char **commands, int sock)
{
	int			ret;

	ret = 0;
	if (ft_strcmp(commands[0], "ls") == 0)
		send_command(T_LS, commands[0], sock, 0);
	else if (ft_strcmp(commands[0], "pwd") == 0)
		send_command(T_PWD, commands[0], sock, 0);
	else if (ft_strcmp(commands[0], "cd") == 0)
	{
		if (commands[1] && ft_strlen(commands[1]) > 0)
			send_command(T_CD, commands[1], sock, 0);
		else
			send_command(T_CD, "/", sock, 0);
	}
	else if (ft_strcmp(commands[0], "quit") == 0)
		return (-1);
	else
		ret = parse_command2(commands, sock);
	free_tab(commands);
	return (ret);
}
