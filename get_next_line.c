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

int		check_for_full_leftover_line(t_list *head, char **result);
ssize_t	find_endofline(t_list *cur);
int		add_new_node(t_list *cur);
void	free_list(t_list *head);
void	clear_static(t_list *head);
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
		if (!check_for_full_leftover_line(&head, &result))
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
		cur->bytes_unsaved = read(fd, cur->buf, BUFFER_SIZE);
		if (cur->bytes_unsaved == -1)
		{
			// read error handling
			free_list(&head);
			clear_static(&head);
			return (NULL);
		}
		if (cur->bytes_unsaved != BUFFER_SIZE)
		{
			if (head.bytes_unsaved == 0)
				return (NULL);
			cur->endoffile = 1;
		}
		cur->buf[cur->bytes_unsaved] = '\0';
		cur->line_end = find_endofline(cur);
		if (cur->line_end == -1 && !cur->endoffile)
		{
			if (!add_new_node(cur))
			{
				// malloc error handling
				free_list(&head);
				return (NULL);
			}
		}
		cur = cur->next;
	}
	// start at head and copy while cur->next && cur->line_end == -1
	cur = &head;
	result_size = 0;
	// count result_size for malloc
	while (cur->next)
	{
		result_size += cur->bytes_unsaved;
		cur = cur->next;
	}
	result_size += cur->line_end + 1;
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
		j = 1;
		while (j <= cur->bytes_unsaved)
			result[i++] = cur->buf[cur->line_end + j++];
		cur = cur->next;
	}
	j = 0;
	while (j <= cur->line_end)
		result[i++] = cur->buf[j++];
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
		if (!head.bytes_unsaved)
		{
			head.line_end = -1;
			head.buf[0] = '\0';
		}
	}
	head.endoffile = 0;
	return (result);
}

int	check_for_full_leftover_line(t_list *head, char **result)
{
	ssize_t	i;
	ssize_t	new_newline_pos;
	ssize_t	result_size;

	new_newline_pos = find_endofline(head);
	if (new_newline_pos != -1)
	{
		result_size = new_newline_pos - head->line_end;
		*result = (char *) malloc(result_size + 1);
		if (!*result)
			return (0);
		i = 0;
		head->line_end++;
		while (i < result_size)
		{
			(*result)[i] = head->buf[head->line_end + i];
			i++;
		}
		(*result)[i] = '\0';
		head->bytes_unsaved -= result_size;
		head->line_end = new_newline_pos;
	}
	else
		*result = NULL;
	return (1);
}

ssize_t	find_endofline(t_list *cur)
{
	ssize_t	i;

	i = cur->line_end + 1;
	while (cur->buf[i])
	{
		if (cur->buf[i] == '\n')
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
	cur->next->bytes_unsaved = 0;
	cur->next->line_end = -1;
	cur->next->endoffile = 0;
	cur->next->next = NULL;
	return (1);
}

void	save_leftover(t_list *head, t_list *cur)
{
	ssize_t	i;
	ssize_t	j;

	i = 0;
	j = cur->line_end + 1;
	while (j < cur->bytes_unsaved)
		head->buf[i++] = cur->buf[j++];
	head->buf[i] = '\0';
	head->bytes_unsaved = i;
	head->line_end = -1;	// 0 or -1?
	head->endoffile = cur->endoffile;	// redundant atm bc I set it to 0 afterwards
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

void	clear_static(t_list *head)
{
	ssize_t	i;

	i = 0;
	while (head->buf[i])
	  	head->buf[i++] = '\0';
	head->bytes_unsaved = 0;
	head->line_end = -1;
	head->endoffile = 0;
	head->next = NULL;
}
