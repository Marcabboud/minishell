# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eden <eden@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/04 19:41:59 by redrouic          #+#    #+#              #
#    Updated: 2025/03/26 11:01:14 by eden             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC		=	src/main.c\
			src/utils.c\
			src/free.c\
			src/cmd_builder.c\
			src/cmd_redir_parser.c\
			src/parsing/array_builder.c\
			src/parsing/env_list_builder.c\
			src/parsing/syntax_checker.c\
			src/parsing/env_expander.c\
			src/redir/redir_executor.c\
			src/redir/redir_parser.c\
			src/redir/heredoc_handler.c\
			src/redir/redir_utils.c\
			src/builtins/ft_export.c\
			src/builtins/ft_unset.c\
			src/builtins/ft_cd.c\
			src/builtins/builtin_cmds.c\
			src/executor/executor.c\
			src/executor/ft_exec.c\
			src/executor/ft_one.c\
			src/executor/ft_pipes.c

SLiB	=	$(wildcard lib/*.c)

OBJ_DIR	=	objet

OBJ		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

LOBJ	=	$(addprefix $(OBJ_DIR)/, $(notdir $(SLiB:.c=.o)))

CC		=	cc

BIN		=	minishell

NAME	=	libft.a

RL		=	-lreadline

RM		=	rm -rf

CFLAGS	+=	-Wall -Wextra -Werror -g3

GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
BLUE = \033[0;34m
NC = \033[0m
FLASH = \033[6m
WHITE = \033[0;37m

all: $(NAME) $(BIN)
	@printf "$(WHITE)[OK] Compilation minishell done\n"

$(NAME): $(LOBJ)
	@echo "\n$(GREEN)[OK] Creating executable $(NAME)"
	@ar rc $(NAME) $(LOBJ)

define make_rule
$(OBJ_DIR)/$(notdir $(1:.c=.o)): $(1)
	@mkdir -p $(OBJ_DIR)
	@printf "\r\033[K$(BLUE)Compiling $(notdir $(1))$(NC)"
	@$(CC) $(CFLAGS) -c $(1) -o $(OBJ_DIR)/$(notdir $(1:.c=.o))
endef

$(foreach file, $(SRC), $(if $(strip $(file)), $(eval $(call make_rule,$(file)))))
$(foreach file, $(SLiB), $(if $(strip $(file)), $(eval $(call make_rule,$(file)))))

$(BIN): $(OBJ)
	@printf "\n"
	@$(CC) $(CFLAGS) $(OBJ) $(NAME) -o $(BIN) $(RL)

clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(YELLOW)Removing object files\n"

fclean: clean
	@echo "$(YELLOW)Removing executable"
	@$(RM) $(NAME)
	@$(RM) $(BIN)

re: fclean all

.PHONY: all clean fclean re
