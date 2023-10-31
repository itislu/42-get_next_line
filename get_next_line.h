/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldulling <ldulling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 12:28:58 by ldulling          #+#    #+#             */
/*   Updated: 2023/10/31 16:50:20 by ldulling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 128
# endif

typedef struct s_list
{
	char			buf[BUFFER_SIZE + 1];
	ssize_t			bytes_unsaved;
	ssize_t			line_end;
	char			endoffile;
	struct s_list	*next;
}	t_list;

/* get_next_line.c */
char	*get_next_line(int fd);
int		check_for_full_leftover_line(t_list *head, char **result);
int		read_until_endofline(t_list *head, int fd);
char	*copy_into_result(t_list *head, t_list **cur, size_t *result_size);
void	save_leftover(t_list *head, t_list *cur, ssize_t result_size);

/* get_next_line_utils.c */
int		add_new_node(t_list *cur);
void	clear_static(t_list *head);
size_t	count_result_size(t_list *cur);
ssize_t	find_endofline(t_list *cur);
void	free_list(t_list *head);

#endif