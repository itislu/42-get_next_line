/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldulling <ldulling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 15:06:25 by ldulling          #+#    #+#             */
/*   Updated: 2023/11/01 21:52:31 by ldulling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_list	head = {{'\0'}, 0, NO_NL, 0, NULL};
	t_list			*cur;
	char			*result;
	size_t			result_size;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (head.bytes_unsaved)
	{
		if (!check_for_full_leftover_line(&head, &result))
			return (NULL);
		if (result)
			return (result);
		if (!add_new_node(&head))
			return (NULL);
	}
	if (!read_until_endofline(&head, fd))
		return (NULL);
	cur = &head;
	result_size = 0;
	result = copy_into_result(&head, &cur, &result_size);
	if (!result)
		return (NULL);
	save_leftover(&head, cur, (ssize_t) result_size);
	return (result);
}

int	check_for_full_leftover_line(t_list *head, char **result)
{
	ssize_t	i;
	ssize_t	new_newline_pos;
	ssize_t	result_size;

	new_newline_pos = find_endofline(head);
	if (new_newline_pos != NO_NL)
	{
		result_size = new_newline_pos - head->line_end;
		*result = (char *) malloc(result_size + 1);
		if (!*result)
			return (0);
		i = 0;
		head->line_end++;
		while (i < result_size)
		{
			(*result)[i++] = head->buf[head->line_end];
			head->buf[head->line_end++] = '\0';
		}
		(*result)[i] = '\0';
		head->bytes_unsaved -= result_size;
		head->line_end = new_newline_pos;
	}
	else
		*result = NULL;
	return (1);
}

int	read_until_endofline(t_list *head, int fd)
{
	t_list	*cur;

	if (head->bytes_unsaved)
		cur = head->next;
	else
		cur = head;
	while (cur)
	{
		cur->bytes_unsaved = read(fd, cur->buf, BUFFER_SIZE);
		if (cur->bytes_unsaved == -1)
			return (free_list(head), clear_static(head), 0);
		if (cur->bytes_unsaved != BUFFER_SIZE)
		{
			if (head->bytes_unsaved == 0)
				return (0);
			cur->endoffile = 1;
		}
		cur->buf[cur->bytes_unsaved] = '\0';
		cur->line_end = find_endofline(cur);
		if (cur->line_end == NO_NL && !cur->endoffile)
			if (!add_new_node(cur))
				return (free_list(head), clear_static(head), 0);
		cur = cur->next;
	}
	return (1);
}

char	*copy_into_result(t_list *head, t_list **cur, size_t *result_size)
{
	char	*result;
	size_t	i;
	ssize_t	j;

	*result_size = count_result_size(*cur);
	result = (char *) malloc(*result_size + 1);
	if (!result)
		return (free_list(head), clear_static(head), NULL);
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

void	save_leftover(t_list *head, t_list *cur, ssize_t result_size)
{
	ssize_t	i;
	ssize_t	j;

	i = 0;
	if (cur != head)
	{
		j = cur->line_end + 1;
		while (j < cur->bytes_unsaved)
			head->buf[i++] = cur->buf[j++];
		j = i;
		if (i <= head->line_end)
			i = head->line_end + 1;
		while (head->buf[i])
			head->buf[i++] = '\0';
		head->bytes_unsaved = j;
		head->line_end = NO_NL;
		free_list(head);
		return ;
	}
	if (result_size == head->bytes_unsaved)
		return (clear_static(head));
	while (i < result_size)
		head->buf[i++] = '\0';
	head->bytes_unsaved -= result_size;
}
