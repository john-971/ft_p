/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 16:35:58 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/30 16:36:04 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_p.h"

void				manage_login(int sock, char *msg_value, int max_try)
{
	char			*u_input;

	if (max_try == MAX_TRY)
	{
		print_error(ERR_MAXTRY);
		exit(EXIT_SUCCESS);
	}
	u_input = NULL;
	if (ft_memcmp(msg_value, V_LOGIN, ft_strlen(V_LOGIN)) == 0)
		ft_putstr("Login :");
	else if (ft_memcmp(msg_value, V_PASS, ft_strlen(V_PASS)) == 0)
		ft_putstr("Password :");
	get_next_line(0, &u_input);
	if (u_input && ft_strlen(u_input) > 0)
	{
		send_command(T_LOG, u_input, sock, 0);
		free(u_input);
	}
	else
	{
		if (u_input)
			free(u_input);
		manage_login(sock, msg_value, max_try + 1);
	}
}

int					parse_msg2(t_trame trame)
{
	if (ft_memcmp(trame.type, T_MSG, CMD_SIZE) == 0)
	{
		if (trame.type_msg == T_MSG_KO)
		{
			print_error(trame.value);
			if (ft_memcmp(trame.value, BAD_LOG, ft_strlen(BAD_LOG)) == 0)
				return (1);
		}
		else
			print_succes(trame.value);
	}
	else if (ft_memcmp(trame.type, T_BYE, CMD_SIZE) == 0)
	{
		print_error(ERR_MAXTRY);
		return (-1);
	}
	else
		print_error("Type de commande inconnu");
	return (0);
}

int					parse_msg(t_trame trame, int sock, t_info *info)
{
	int				ret;

	ret = 1;
	if (ft_memcmp(trame.type, T_LOG, CMD_SIZE) == 0)
		manage_login(sock, trame.value, 0);
	else if (ft_memcmp(trame.type, T_LS, CMD_SIZE) == 0)
		manage_ls(sock);
	else if (ft_memcmp(trame.type, T_PWD, CMD_SIZE) == 0)
		printf("%s\n", trame.value);
	else if (ft_memcmp(trame.type, T_CD, CMD_SIZE) == 0)
	{
		if (info->path)
			free(info->path);
		info->path = ft_strdup(trame.value);
	}
	else if (ft_memcmp(trame.type, T_GET, CMD_SIZE) == 0)
	{
		return (manage_get(trame, sock));
	}
	else
		ret = parse_msg2(trame);
	return (ret);
}
