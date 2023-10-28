/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldulling <ldulling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 15:06:25 by ldulling          #+#    #+#             */
/*   Updated: 2023/10/28 15:17:37 by ldulling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	ssize_t			bytes_unsaved;
	struct s_list	*next;
}	t_list;

typedef struct	s_head
{
	char	str[BUFFER_SIZE + 1];
	char	leftover[BUFFER_SIZE];
	ssize_t	newline_pos;
	ssize_t	bytes_unsaved;
	t_list	*next;
}	t_head;

int		check_leftovers(t_list *head, char **result);
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
	if (head.bytes_unsaved)
	{
		if (head.newline_pos != -1)
			if (!check_leftovers(&head, &result))
				return (NULL);
		if (result)
			return (result);
		if (!add_new_node(&head))
			return (NULL);
		cur = head.next;
	}
	else
		cur = &head;
	// read loop
	while (cur)
	{
		cur->bytes_unsaved = read(fd, cur->str, BUFFER_SIZE);
		if (cur->bytes_unsaved == -1)
		{
			//read error handling
		}
		cur->str[cur->bytes_unsaved] = '\0';
		cur->newline_pos = find_newline(cur->str);
		if (cur->newline_pos == -1)
		{
			if (!add_new_node(cur))
			{
				//malloc error handling
			}
		}
		else if (cur->newline_pos != cur->bytes_unsaved)	// for end of file
		{
			// copy everything after newline into head.leftover
			save_leftover(&head, cur);	//currently overwrites what is in head already
		}
		cur = cur->next;
	}
	// start at head and copy while cur->next && cur->newline_pos == -1
	cur = &head;
	result_size = 0;
	// count result_size for malloc
	while (cur->next)
	{
		result_size += cur->bytes_unsaved;
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
		while (j < cur->bytes_unsaved)	//would be while (cur->str[j]) with BUFFER_SIZE + 1 in struct
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
		save_leftover(&head, cur);
		//TODO: free all nodes after head
	}
	return (result);
}

int	check_leftovers(t_list *head, char **result)
{
	size_t	i;
	ssize_t	new_newline_pos;
	size_t	result_size;

	new_newline_pos = find_newline(&head->str[head->newline_pos]);
	if (new_newline_pos != -1)
	{
		result_size = new_newline_pos - head->newline_pos + 1;
		*result = (char *) malloc(result_size + 1);
		if (!*result)
			return (0);
		i = 0;
		while (i < result_size)
			*result[i++] = head->str[head->newline_pos++];
		*result[i] = '\0';
		head->bytes_unsaved -= result_size;
	}
	else
		*result = NULL;
	return (1);
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
	cur->next->newline_pos = -1;
	cur->next->next = NULL;
	return (1);
}

void	save_leftover(t_list *head, t_list *cur)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = cur->newline_pos + 1;
	while (j < cur->bytes_unsaved)
		head->str[i++] = cur->str[j++];
	head->str[i] = '\0';
	head->newline_pos = -1;	// 0 or -1?
	head->bytes_unsaved = i;
	return ;
}