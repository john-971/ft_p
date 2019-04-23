#include "../includes/ft_p.h"

char			*get_name_from_path(char *path)
{
	char 		*name;
	int 		i;
	int 		nb_lvl;

	name = path;
	while((path = ft_memchr(path, '/', ft_strlen(path))) != NULL)
	{
		path++;
		name = path;
//		printf("DEBUG : get_name_from_path => NAME : %s \n", name);
	}
	printf("DEBUG : get_name_from_path => NAME : %s\n", name);
	return name;
}