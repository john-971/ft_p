/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 16:42:42 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/30 16:42:45 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_p.h"

int					prompt(int sock, t_info info)
{
	char			*u_input;
	int				ret;

	print_prompt(info.path);
	get_next_line(0, &u_input);
	if (ft_strlen(u_input) > 0)
	{
		if ((ret = parse_command(u_input, sock)) == 0)
			return (0);
		if (ret == -1)
		{
			print_succes("adieu monde cruel .·´¯`(>▂<)´¯`·.");
			return (-1);
		}
	}
	prompt(sock, info);
	return (0);
}

void				main_process(int m_sock)
{
	t_trame			trame;
	t_info			info;
	int				ret;

	ft_bzero(&trame, sizeof(t_trame));
	ft_bzero(&info, sizeof(t_info));
	info.path = ft_strdup("/");
	while (1)
	{
		if ((trame = listen_sock(m_sock)).error == 1)
			break ;
		if ((ret = parse_msg(trame, m_sock, &info)) == 0)
		{
			ret = prompt(m_sock, info);
			if (ret == -1)
				break ;
		}
		if (ret == -1)
			break ;
	}
	close(m_sock);
}

int					main(int ac, char **av)
{
	int				port;
	int				m_sock;

	if (ac != 3)
		usage(av[0], 1);
	port = atoi(av[2]);
	m_sock = create_client(av[1], port);
	if (m_sock != -1)
	{
		printf("Connected to %s\n", av[1]);
		main_process(m_sock);
		printf("END OF PROGRAM \n");
	}
	else
		exit(EXIT_FAILURE);
}
