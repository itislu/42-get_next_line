#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 128
#endif

typedef struct	s_list
{
	char			str[BUFFER_SIZE];
	ssize_t			newline_pos;
	ssize_t			start;	//can prolly be combined with newline_pos;
	struct s_list	*next;
}	t_list;

char	*get_next_line(int fd)
{
	static t_list	head;
	t_list			*cur;
	ssize_t			bytes_read;
	char			*result;
	size_t			result_size;
	size_t			i;
	size_t			j;

	cur = &head;
	contains_newline = -1;
	while (newline_pos != -1)
	{
		bytes_read = read(fd, cur->str, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			//read error handling
		}
		cur->newline_pos = find_newline(cur->str, bytes_read);	//TODO
		if (cur->newline_pos == -1)
		{
			//add new node and change cur to it
		}
		else
			break ;	//prolly not even necessary bc of the while condition
	}
	// start at head and copy while cur->next && cur->newline_pos == -1
	cur = &head;
	// count result_size for malloc
	while (cur->next)
	{
		result_size += BUFFER_SIZE;
		cur = cur->next;
	}
	result_size += cur->newline_pos + 1;
	result = (char *) malloc(result_size + 1);
	if (!result)
	{
		//malloc error handling
	}
	// copy from linked list into result
	cur = &head;
	i = 0;
	while (cur->next)
	{
		j = 0;
		while (j < BUFFER_SIZE)
			result[i++] = cur->str[j++];
		cur = cur->next;
	}
	j = 0;
	while (j <= cur->newline_pos)
		result[i++] = cur->str[j++];
	result[i] = '\0';
	// copy any leftover into the static head
	i = 0;
	while (j < bytes_read)
		head.str[i++] = cur->str[j++];
	head.start = i;
	//free all nodes after head

}