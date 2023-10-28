#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#ifndef FD
# define FD 0
#endif
#define BUFFER_SIZE 1

#include "get_next_line.h"

int	main(void)
{
	char	*result;
	int		fd;

	fd = open("/nfs/homes/ldulling/francinette/tests/get_next_line/fsoares/read_error.txt", O_RDONLY);
	//fd = open("test_short.txt", O_RDONLY);

	/* LOOP */
	// result = get_next_line(fd);
	// printf("%s", result);
	// while (result)
	// {
	// 	free(result);
	// 	result = get_next_line(fd);
	// 	printf("%s", result);
	// }

	/* LINE BY LINE */
	int	iterations = 9;
	for (int i = 0; i < iterations; i++)
	{
		result = get_next_line(fd);
		printf("%d: %s", i + 1, result);
		free(result);
	}

	close(fd);
	return (0);
}