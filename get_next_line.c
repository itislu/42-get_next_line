/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldulling <ldulling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 15:06:25 by ldulling          #+#    #+#             */
/*   Updated: 2023/11/02 15:28:12 by ldulling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_list	*head;
	char			*result;

	if (!initial_check(fd, &head))
		return (NULL);
	if (head->bytes_unsaved)
	{
		if (check_for_full_leftover_line(&head, &result))
			return (result);
		if (!add_new_node(head))
			return (free_list(&head), NULL);
	}
	if (!read_until_endofline(&head, fd))
		return (NULL);
	result = copy_into_result(&head);
	if (!result)
		return (NULL);
	point_head_to_tail(&head);
	if (head->bytes_unsaved == 0)
	{
		free_list(&head);
	}
	return (result);
}

int	check_for_full_leftover_line(t_list **head, char **result)
{
	ssize_t	i;
	ssize_t	new_newline_pos;
	ssize_t	result_size;

	new_newline_pos = find_endofline(*head);
	if (new_newline_pos != NO_NL)
	{
		result_size = new_newline_pos - (*head)->line_end;
		*result = (char *) malloc(result_size + 1);
		if (!*result)
			return (free_list(head), 1);
		i = 0;
		(*head)->line_end++;
		while (i < result_size)
		{
			(*result)[i++] = (*head)->buf[(*head)->line_end++];
		}
		(*result)[i] = '\0';
		(*head)->bytes_unsaved -= result_size;
		(*head)->line_end = new_newline_pos;
		return (1);
	}
	return (0);
}

int	read_until_endofline(t_list **head, int fd)
{
	t_list	*cur;

	if ((*head)->next)
		cur = (*head)->next;
	else
		cur = *head;
	while (cur)
	{
		cur->bytes_unsaved = read(fd, cur->buf, BUFFER_SIZE);
		if (cur->bytes_unsaved == -1 || (*head)->bytes_unsaved == 0)
			return (free_list(head), 0);
		if (BUFFER_SIZE <= SSIZE_MAX && cur->bytes_unsaved != BUFFER_SIZE)
		{
			cur->endoffile = 1;
		}
		cur->buf[cur->bytes_unsaved] = '\0';
		cur->line_end = find_endofline(cur);
		if (cur->line_end == NO_NL && !cur->endoffile)
		{
			if (!add_new_node(cur))
				return (free_list(head), 0);
		}
		cur = cur->next;
	}
	return (1);
}

char	*copy_into_result(t_list **head)
{
	t_list	*cur;
	char	*result;
	size_t	i;
	ssize_t	j;

	result = (char *) malloc(count_result_size(*head) + 1);
	if (!result)
		return (free_list(head), NULL);
	cur = *head;
	i = 0;
	while (cur->next)
	{
		j = cur->line_end + 1;
		while (cur->buf[j])
			result[i++] = cur->buf[j++];
		cur = cur->next;
	}
	j = 0;
	while (j <= cur->line_end)
	{
		result[i++] = cur->buf[j++];
	}
	result[i] = '\0';
	return (result);
}

void	point_head_to_tail(t_list **head)
{
	t_list	*cur;

	while ((*head)->next)
	{
		cur = (*head);
		(*head) = (*head)->next;
		free(cur);
	}
	(*head)->bytes_unsaved -= (*head)->line_end + 1;
	return ;
}
