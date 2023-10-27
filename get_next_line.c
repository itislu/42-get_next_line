#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 128
#endif

typedef struct	s_list
{
	char			str[BUFFER_SIZE + 1];	//prolly better with + 1 for '\0'
	ssize_t			newline_pos;
	ssize_t			bytes_read;
	struct s_list	*next;
}	t_list;

ssize_t	find_newline(char *str);
int		add_new_node(t_list *cur);
void	save_leftover(t_list *head, t_list *cur);

char	*get_next_line(int fd)
{
	static t_list	head = {{'\0'}, -1, 0, NULL};
	t_list			*cur;
	char			*result;
	size_t			result_size;
	size_t			i;
	size_t			j;

	//check for leftovers
	if (head.str[0])
	{
		i = 0;
		while (head.newline_pos < head.bytes_read)
	}
	cur = &head;
	while (cur)
	{
		cur->bytes_read = read(fd, cur->str, BUFFER_SIZE);
		if (cur->bytes_read == -1)
		{
			//read error handling
		}
		cur->str[cur->bytes_read] = '\0';
		cur->newline_pos = find_newline(cur->str);
		if (cur->newline_pos == -1)
		{
			// add new node and change cur to it
			if (!add_new_node(cur))
			{
				//malloc error handling
			}
		}
		else if (cur->newline_pos != cur->bytes_read)
		{
			// copy everything after newline into head.leftover
			save_leftover(&head, cur);
		}
		cur = cur->next;
	}
	// start at head and copy while cur->next && cur->newline_pos == -1
	cur = &head;
	result_size = 0;
	// count result_size for malloc
	while (cur->next)
	{
		result_size += cur->bytes_read;
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
		while (j < cur->bytes_read)	//would be while (cur->str[j]) with BUFFER_SIZE + 1 in struct
			result[i++] = cur->str[j++];
		cur = cur->next;
	}
	j = 0;
	while (j <= cur->newline_pos)
		result[i++] = cur->str[j++];
	result[i] = '\0';
	// copy any leftover into the static head
	if (cur != &head)
	{
		i = 0;
		while (j < cur->bytes_read)
			head.str[i++] = cur->str[j++];
		head.str[i] = '\0';
		//free all nodes after head
	}

}

ssize_t	find_newline(char *str)
{
	ssize_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

int	add_new_node(t_list *cur)
{
	cur->next = (t_list *) malloc(sizeof(t_list));
	if (!cur->next)
		return (0);
	cur->newline_pos = -1;
	cur->next = NULL;
	return (1);
}

void	save_leftover(t_list *head, t_list *cur)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = cur->newline_pos + 1;
	while (cur->str[j])
		head->str[i++] = cur->str[j++];
	return ;
}