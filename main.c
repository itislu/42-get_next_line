#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#ifndef FD
# define FD 0
#endif

int	main(void)
{
	char	*result;
	int		fd;

	fd = open("/nfs/homes/ldulling/francinette/tests/get_next_line/gnlTester/files/nl", O_RDONLY);
	//fd = open("test_short.txt", O_RDONLY);
	result = get_next_line(fd);
	printf("%s", result);
	//while (result)
	//{
		free(result);
	//	result = get_next_line(fd);
	//	printf("%s", result);
	//}
	result = get_next_line(fd);
	printf("%s", result);
	//while (result)
	//{
		free(result);
	close(fd);
	return (0);
}