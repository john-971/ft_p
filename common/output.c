#include "../includes/ft_p.h"

void					print_error(char *msg)
{
	ft_putstr("\033[0;31m");
	printf("ERROR /!\\ %s /!\\ \n", msg);
	ft_putstr("\033[0m");
}

void					print_succes(char *msg)
{
	ft_putstr("\033[0;32m");
	printf("SUCCESS %s \n", msg);
	ft_putstr("\033[0m");
}

void					print_prompt(char *path)
{
	ft_putstr("\033[0;33m");
	ft_putchar('(');
	ft_putstr(path);
	ft_putstr(")ft_p:>");
	ft_putstr("\033[0m");
}

void			print_status_bar(off_t c_size, off_t f_size)
{
	int			part;
	int 		i;
	char 		*percent;

	i = 0;
	part = (100 * c_size) / f_size;
	percent = ft_itoa(part);
//	printf("PERCENT %s : TOTAL SIZE %llu\n", percent, f_size);
	ft_putchar('\r');
	ft_putstr("\033[0;44m");
	ft_putstr(percent);
	ft_putchar('%');
	while (i < part)
	{
		if (!(i % 2))
			ft_putchar(' ');
		i++;
	}
	free(percent);
	ft_putstr("\033[0m");
}

void					usage(char *str, int src)
{
	if (src == SERVER)
		printf("usage : %s <port>", str);
	else
		printf("usage : %s <address> <port>", str);
	exit(EXIT_FAILURE);
}