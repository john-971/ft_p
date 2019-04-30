/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 09:29:52 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/30 09:29:58 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_p.h"

void				manage_ls_child(int sock)
{
	char			*test[3];

	test[0] = "ls";
	test[1] = "-la";
	test[2] = NULL;
	send_command(T_LS, OK, sock, 0);
	dup2(sock, STDOUT_FILENO);
	dup2(sock, STDERR_FILENO);
	if (execv("/bin/ls", test) != -1)
	{
		close(sock);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(sock);
		exit(EXIT_FAILURE);
	}
}

void				ls_command(int sock)
{
	int				child;
	int				exit_status;

	exit_status = -1;
	if ((child = fork()) > 0)
	{
		wait4(child, &exit_status, 0, NULL);
		if (exit_status == RET_EXIT_FAIL)
			send_message(T_MSG_KO, ERROR_EXEC, sock);
		else
		{
			if (wait_response(sock) == 0)
				send_message(T_MSG_OK, LS_GOOD, sock);
		}
	}
	else if (child == 0)
		manage_ls_child(sock);
	else
		send_message(T_MSG_KO, ERROR_EXEC, sock);
}

int					manage_command(int cs, t_trame trame, t_info *info)
{
	if (ft_memcmp(trame.type, T_LS, CMD_SIZE) == 0)
		ls_command(cs);
	else if (ft_memcmp(trame.type, T_PWD, CMD_SIZE) == 0)
		format_path(info, cs, T_PWD);
	else if (ft_memcmp(trame.type, T_CD, CMD_SIZE) == 0)
		cd_command(cs, trame, info);
	else if (ft_memcmp(trame.type, T_GET, CMD_SIZE) == 0)
		return (get_command(cs, trame));
	else if (ft_memcmp(trame.type, T_PUT, CMD_SIZE) == 0)
		return (put_command(cs, trame));
	else
		send_message(T_MSG_KO, COMMAND_NOT_FOUND, cs);
	return (0);
}
