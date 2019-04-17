#include "../includes/ft_p.h"

void					print_error(char *msg)
{
	printf("\033[0;31m");
	printf("/!\\ %s /!\\ \n", msg);
	printf("\033[0m");
}

void					print_succes(char *msg)
{
	printf("\033[0;32m");
	printf("%s \n", msg);
	printf("\033[0m");
}