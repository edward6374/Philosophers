# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/19 16:43:11 by vduchi            #+#    #+#              #
#    Updated: 2023/07/07 12:25:13 by vduchi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#=-=-=-=-=-=-=- COLORS DEFINITION =-=-=-=-=-=-=-=-=-#

DEL_LINE		=	\033[2K
ITALIC			=	\033[3m
BOLD			=	\033[1m
DEF_COLOR		=	\033[0;39m
GRAY			=	\033[0;90m
RED				=	\033[0;91m
GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m
BLUE			=	\033[0;94m
MAGENTA			=	\033[0;95m
CYAN			=	\033[0;96m
WHITE			=	\033[0;97m
BLACK			=	\033[0;99m
ORANGE			=	\033[38;5;209m
BROWN			=	\033[38;2;184;143;29m
DARK_GRAY		=	\033[38;5;234m
MID_GRAY		=	\033[38;5;245m
DARK_GREEN		=	\033[38;2;75;179;82m
DARK_YELLOW		=	\033[38;5;143m

#=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-#

NAME			=	philo

SRCS			=	srcs/main.c
SRCS			+=	srcs/utils.c
SRCS			+=	srcs/timers.c
SRCS			+=	srcs/philos.c

OBJS			=	$(patsubst $(SRC_DIR)/%, $(OBJS_DIR)/%, $(SRCS:.c=.o))
DEPS			=	$(patsubst $(SRC_DIR)/%, $(DEPS_DIR)/%, $(SRCS:.c=.d))

SRC_DIR			=	srcs
INC_DIR			=	inc
OBJS_DIR		=	.obj
DEPS_DIR		=	.dep

#=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-#

CFLAGS			+= 	-Wall -Werror -Wextra -g -O3 $(addprefix -I , $(INC_DIR))
DFLAGS			=	-MMD -MP -MF $(DEPS_DIR)/$*.d

#=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-#

CC				=	gcc
RM				=	rm -rf
MKDIR			=	mkdir -p

#=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-#

$(OBJS_DIR)/%.o	:	$(SRC_DIR)/%.c
	@$(CC) -c $< $(CFLAGS) $(DFLAGS) -o $@
	@echo "$(YELLOW)$(patsubst $(SRC_DIR)/%,%, $<)   \tcompiled!$(DEF_COLOR)"

all				:	directories
	@$(MAKE) $(NAME)

$(NAME)			::
	@echo "$(MAGENTA)\nChecking philosophers...$(DEF_COLOR)"

$(NAME)			::	$(OBJS) 
	@$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@
	@echo "$(ORANGE)Compiling philosophers exec...$(DEF_COLOR)"

$(NAME)			::
	@echo "$(GREEN)Philosophers executable ready!$(DEF_COLOR)"

directories	:
	@$(MKDIR) $(OBJS_DIR)
	@$(MKDIR) $(DEPS_DIR)

clean			:
	@$(RM) $(OBJS_DIR)
	@$(RM) $(DEPS_DIR)

fclean			:	clean
	@$(RM) $(NAME)
	@echo "$(BLUE)\nPhilosophers cleaned!$(DEF_COLOR)"

re				:	fclean all

-include $(DEPS)

.PHONY: all clean fclean re

#
#valgrind:
#	valgrind --leak-check=yes --show-leak-kinds=all "nombre del executable" "nombre de argumentos"
#	valgrind --leak-check=yes --track-origins=yes "nombre del executable" "nombre de argumentos"
#
#	gcc $(CFLAGS) -fsanitize=address -g -O3 -fno-omit-frame-pointer $(LDFLAGS) $(SRCS) -o $@
