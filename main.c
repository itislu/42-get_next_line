#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>

#ifndef FD
# define FD 0
#endif

//#define BUFFER_SIZE 10

#include "get_next_line.h"

int	main(void)
{
	char	*result;
	int		fd;
	int		iterations;

	// fd = open("/nfs/homes/ldulling/francinette/tests/get_next_line/fsoares/empty.txt", O_RDONLY);

	// close(fd);
	//fd = open("/nfs/homes/ldulling/francinette/tests/get_next_line/gnlTester/files/42_with_nl", O_RDONLY);

	fd = open("test_short.txt", O_RDONLY);

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
	iterations = 2;
	for (int i = 0; i < iterations; i++)
	{
		result = get_next_line(fd);
		printf("|%d: %s", i + 1, result);
		free(result);
	}
	close(fd);

	// With BUFFER_SIZE of 10, if there's still a full line in the buffer, but before getting it the file gets closed, it would still return the full line from the buffer once it's called again.
	fd = open("test_short.txt", O_RDONLY);
	iterations = 3;
	for (int i = 0; i < iterations; i++)
	{
		result = get_next_line(fd);
		printf("|%d: %s", i + 1, result);
		free(result);
	}
	close(fd);
	return (0);
}