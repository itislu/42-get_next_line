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

#include "get_next_line.h"
#include <stdio.h>

int		check_leftovers(t_list *head, char **result);
ssize_t	find_endofline(t_list *cur);
int		add_new_node(t_list *cur);
void	free_list(t_list *head);
void	save_leftover(t_list *head, t_list *cur);

char	*get_next_line(int fd)
{
	static t_list	head = {{'\0'}, 0, -1, 0, NULL};
	t_list			*cur;
	char			*result;
	size_t			result_size;
	size_t			i;
	ssize_t			j;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	// check for leftovers
	if (head.bytes_unsaved)
	{
		if (!check_leftovers(&head, &result))
			return (NULL);
		if (result)
			return (result);
		if (!add_new_node(&head))
			return (NULL);
		cur = head.next;
	}
	else if (head.endoffile)
		return (NULL);
	else
		cur = &head;
	// read loop
	while (cur)
	{
		cur->bytes_unsaved = read(fd, cur->str, BUFFER_SIZE);
		if (cur->bytes_unsaved == -1)
		{
			// read error handling
			free_list(&head);
			return (NULL);
		}
		// if (cur->bytes_unsaved == 0)
		// {
		// 	head.endoffile = 1;
		// 	break ;
		// }
		cur->str[cur->bytes_unsaved] = '\0';
		if (cur->bytes_unsaved != BUFFER_SIZE)
		{
			cur->endoffile = 1;
			if (head.endoffile && head.bytes_unsaved == 0)
				return (NULL);
		}
		cur->newline_pos = find_endofline(cur);
		if (cur->newline_pos == -1 && !cur->endoffile)
		{
			// if (cur->bytes_unsaved != BUFFER_SIZE)
			// 	cur->newline_pos = cur->bytes_unsaved - 1;
			if (!add_new_node(cur))
			{
				// malloc error handling
				free_list(&head);
				return (NULL);
			}
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
		// malloc error handling
		free_list(&head);
		return (NULL);
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
		free_list(&head);
	}
	else
	{
		head.bytes_unsaved -= result_size;
		head.newline_pos = -1;
	}
	return (result);
}

int	check_leftovers(t_list *head, char **result)
{
	size_t	i;
	ssize_t	new_newline_pos;
	size_t	result_size;

	new_newline_pos = find_endofline(head);
	if (new_newline_pos != -1)
	{
		result_size = new_newline_pos - head->newline_pos;
		*result = (char *) malloc(result_size + 1);
		if (!*result)
			return (0);
		i = 0;
		head->newline_pos++;
		while (i < result_size)
		{
			(*result)[i] = head->str[head->newline_pos + i];
			i++;
		}
		(*result)[i] = '\0';
		head->newline_pos = new_newline_pos;
		head->bytes_unsaved -= result_size;
	}
	else
		*result = NULL;
	return (1);
}

ssize_t	find_endofline(t_list *cur)
{
	ssize_t	i;

	i = cur->newline_pos + 1;
	while (cur->str[i])
	{
		if (cur->str[i] == '\n')
			return (i);
		i++;
	}
	if (cur->endoffile)
		return (i - 1);
	else
	 	return (-1);
}

int	add_new_node(t_list *cur)
{
	cur->next = (t_list *) malloc(sizeof(t_list));
	if (!cur->next)
		return (0);
	cur->next->endoffile = 0;
	cur->next->newline_pos = -1;
	cur->next->next = NULL;
	return (1);
}

void	save_leftover(t_list *head, t_list *cur)
{
	size_t	i;
	ssize_t	j;

	i = 0;
	j = cur->newline_pos + 1;
	while (j < cur->bytes_unsaved)
		head->str[i++] = cur->str[j++];
	head->str[i] = '\0';
	head->newline_pos = -1;	// 0 or -1?
	head->bytes_unsaved = i;
	head->endoffile = cur->endoffile;
	return ;
}

void	free_list(t_list *head)
{
	t_list	*cur;

	while (head->next)
	{
		cur = head->next;
		head->next = cur->next;
		free(cur);
	}
	return ;
}