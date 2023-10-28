NAME		=	get_next_line
I			=	./
D			=	build/dep/
O			=	build/obj/

SRC			=	main.c \
				get_next_line.c

CC			=	cc
CFLAGS		=	-g -Wall -Wextra -Werror $(foreach X,$I,-I$X)

DEP			=	$(SRC:%.c=$D%.d)
OBJ			=	$(SRC:%.c=$O%.o)

.PHONY:			all bonus cleandep cleanobj clean fclean re norminette

all:			$(NAME)

$(NAME):		$(OBJ)
				$(CC) $(CFLAGS) $^ -o $@

bonus:			all

$(OBJ): $O%.o:	%.c | $O
				$(CC) $(CFLAGS) -c $< -o $@

$(DEP): $D%.d:	%.c | $D
				@$(CC) $(CFLAGS) -MM -MP -MF $@ -MT "$O$*.o $@" $<

$O $D:
				@mkdir -p $@

cleandep:
				rm -rf $D

cleanobj:
				rm -rf $O

clean:			cleandep cleanobj
				rm -rf build/

fclean:			clean
				rm -f $(NAME)

re:				fclean all

norminette:
				norminette -R CheckForbiddenSourceHeader
				norminette -R CheckDefine $I*.h

ifeq ($(filter $(MAKECMDGOALS),cleandep cleanobj clean fclean),)
-include 		$(DEP)
endif