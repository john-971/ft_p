#include "../includes/ft_p.h"

char	*get_error()
{
	if (errno == ENOTDIR)
		return (ERR_NOTDIR);
	if (errno == EACCES)
		return (ERR_ACL);
	if (errno == ENOENT)
		return (ERR_ENOENT);
	if (errno == ENAMETOOLONG)
		return (ERR_NAMETOOLONG);
	else
		return (strerror(errno));
}