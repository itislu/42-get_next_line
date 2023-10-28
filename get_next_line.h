#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct	s_list
{
	char			str[BUFFER_SIZE + 1];
	char			endoffile;
	ssize_t			newline_pos;
	ssize_t			bytes_unsaved;
	struct s_list	*next;
}	t_list;

// Function prototypes here
char	*get_next_line(int fd);

#endif