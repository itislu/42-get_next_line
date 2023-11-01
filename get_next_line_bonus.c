/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldulling <ldulling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 15:06:25 by ldulling          #+#    #+#             */
/*   Updated: 2023/11/01 19:57:31 by ldulling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static t_list	*head[FD_AMOUNT];
	t_list			*cur;
	char			*result;

	if (!initial_check(fd, &head[fd]))
		return (NULL);
	if (head[fd]->bytes_unsaved)
	{
		if (check_for_full_leftover_line(head[fd], &result))
			return (free_list(&head[fd]), result);
		if (!add_new_node(head[fd]))
		{
			head[fd]->bytes_unsaved = 0;
			return (free_list(&head[fd]), NULL);
		}
	}
	cur = head[fd];
	if (!read_until_endofline(head[fd], fd, cur))
		return (free_list(&head[fd]), NULL);
	result = copy_into_result(head[fd], &cur);
	if (!result)
		return (free_list(&head[fd]), NULL);
	if (save_leftover(head[fd], cur))
		free_list(&head[fd]);
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
		{
			head->bytes_unsaved = 0;
			return (1);
		}
		i = 0;
		head->line_end++;
		while (i < result_size)
			(*result)[i++] = head->buf[head->line_end++];
		(*result)[i] = '\0';
		head->bytes_unsaved -= result_size;
		head->line_end = new_newline_pos;
		return (1);
	}
	return (0);
}

int	read_until_endofline(t_list *head, int fd, t_list *cur)
{
	if (head->bytes_unsaved)
		cur = head->next;
	while (cur)
	{
		cur->bytes_unsaved = read(fd, cur->buf, BUFFER_SIZE);
		if (head->bytes_unsaved == 0 || cur->bytes_unsaved == -1)
		{
			head->bytes_unsaved = 0;
			return (0);
		}
		if (cur->bytes_unsaved != BUFFER_SIZE)
			cur->endoffile = 1;
		cur->buf[cur->bytes_unsaved] = '\0';
		cur->line_end = find_endofline(cur);
		if (cur->line_end == -1 && !cur->endoffile)
		{
			if (!add_new_node(cur))
			{
				head->bytes_unsaved = 0;
				return (0);
			}
		}
		cur = cur->next;
	}
	return (1);
}

char	*copy_into_result(t_list *head, t_list **cur)
{
	char	*result;
	size_t	i;
	ssize_t	j;

	result = (char *) malloc(count_result_size(*cur) + 1);
	if (!result)
	{
		head->bytes_unsaved = 0;
		return (NULL);
	}
	(*cur) = head;
	i = 0;
	while ((*cur)->next)
	{
		j = (*cur)->line_end + 1;
		while ((*cur)->buf[j])
			result[i++] = (*cur)->buf[j++];
		(*cur) = (*cur)->next;
	}
	j = 0;
	while (j <= (*cur)->line_end)
		result[i++] = (*cur)->buf[j++];
	result[i] = '\0';
	return (result);
}

int	save_leftover(t_list *head, t_list *cur)
{
	ssize_t	i;
	ssize_t	j;

	if (cur != head)
	{
		i = 0;
		j = cur->line_end + 1;
		while (j < cur->bytes_unsaved)
			head->buf[i++] = cur->buf[j++];
		head->bytes_unsaved = i;
		head->buf[i] = '\0';
		head->line_end = -1;
		return (1);
	}
	head->bytes_unsaved -= head->line_end + 1;
	if (!head->bytes_unsaved)
		return (1);
	return (0);
}
