/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 09:29:28 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/30 09:29:32 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_p.h"

int						test_login(char *login, char *password, int sock)
{
	if (ft_strcmp(login, "john") == 0 && ft_strcmp(password, "bobby") == 0)
	{
		send_message(T_MSG_OK, GOOD_LOG, sock);
		free(login);
		free(password);
		return (0);
	}
	else
	{
		free(login);
		free(password);
		send_message(T_MSG_KO, BAD_LOG, sock);
		return (1);
	}
}

int						manage_login(int sock, int max_try)
{
	t_trame				trame;
	char				*login;
	char				*password;

	if (max_try == MAX_TRY)
	{
		send_command(T_BYE, T_BYE, sock, 0);
		return (-1);
	}
	send_command(T_LOG, V_LOGIN, sock, 0);
	if ((trame = listen_sock(sock)).error == 1)
		return (-1);
	login = ft_strdup(trame.value);
	send_command(T_LOG, V_PASS, sock, 0);
	if ((trame = listen_sock(sock)).error == 1)
		return (-1);
	password = ft_strdup(trame.value);
	if (test_login(login, password, sock) == 0)
		return (0);
	else
		return (manage_login(sock, max_try + 1));
}

void					main_process(int cs)
{
	t_trame				trame;
	t_info				info;

	if (manage_login(cs, 0) == 0)
	{
		info.base_path = NULL;
		info.path = NULL;
		info.base_path = set_path(info.base_path);
		info.path = set_path(info.path);
		printf("\033[0;33mClient [%i], logged \033[0m\n", cs);
		while (1)
		{
			if ((trame = listen_sock(cs)).error == 1)
				break ;
			if (manage_command(cs, trame, &info) == -1)
				break ;
		}
		free(info.base_path);
		free(info.path);
	}
}

void					wait_connexion(int m_sock, uint32_t cslen)
{
	int					cs;
	int					pid;
	struct sockaddr_in	csin;

	while ((cs = accept(m_sock, (struct sockaddr*)&csin, &cslen)) != -1)
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		printf("PID %i\n", pid);
		if (pid == 0)
		{
			printf("\033[0;33mClient connected [%i] \033[0m\n", cs);
			main_process(cs);
			printf("\033[0;33mClose connection for [%i]\033[0m\n", cs);
			close(cs);
		}
	}
}

int						main(int ac, char **av)
{
	int					port;
	int					m_sock;
	uint32_t			cslen;

	cslen = 0;
	if (ac != 2)
		usage(av[0], SERVER);
	port = atoi(av[1]);
	m_sock = create_server(port);
	if (m_sock != -1)
	{
		wait_connexion(m_sock, cslen);
		printf("END OF PROGRAM \n");
	}
	else
		exit(EXIT_FAILURE);
}
