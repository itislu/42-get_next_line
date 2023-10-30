#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 128
# endif

typedef struct	s_list
{
	char			buf[BUFFER_SIZE + 1];
	ssize_t			bytes_unsaved;
	ssize_t			line_end;
	char			endoffile;
	struct s_list	*next;
}	t_list;

// Function prototypes here
char	*get_next_line(int fd);

#endif