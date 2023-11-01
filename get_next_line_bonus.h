/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldulling <ldulling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 12:28:58 by ldulling          #+#    #+#             */
/*   Updated: 2023/11/01 19:57:38 by ldulling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 128
# endif
# define FD_AMOUNT 1024

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
int		read_until_endofline(t_list *head, int fd, t_list *cur);
char	*copy_into_result(t_list *head, t_list **cur);
int		save_leftover(t_list *head, t_list *cur);

/* get_next_line_utils.c */
int		add_new_node(t_list *cur);
size_t	count_result_size(t_list *cur);
ssize_t	find_endofline(t_list *cur);
void	free_list(t_list **head);
int		initial_check(int fd, t_list **head);

#endif