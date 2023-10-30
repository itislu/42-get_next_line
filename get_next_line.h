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
	char			buf[BUFFER_SIZE + 1];	// for some reason + 1 isn't even necessary
	ssize_t			bytes_unsaved;
	ssize_t			newline_pos;	// might actually better be called endofline_pos
	char			endoffile;
	struct s_list	*next;
}	t_list;

// Function prototypes here
char	*get_next_line(int fd);

#endif