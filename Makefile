# define standard colors
BOLD			= \033[1m
BLACK    		= \033[30m    # Black
RED      		= \033[31m    # Red
GREEN    		= \033[32m    # Green
YELLOW   		= \033[33m    # Yellow
BLUE     		= \033[34m    # Blue
MAGENTA  		= \033[35m    # Magenta
CYAN     		= \033[36m    # Cyan
WHITE    		= \033[37m    # White
LIGHT_RED		= \033[91m
LIGHT_GREEN		= \033[92m
LIGHT_CYAN		= \033[96m
RESET			= \033[0m

NAME			= minishell
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -g

DEFS			=
INK				= -I$(LIBFT_DIR) -I$(CURDIR)
LINKS			= -lreadline #-ltinfo

# Libs
LIBFT_DIR		:= libft
LIBFT			= $(LIBFT_DIR)/libft.a

#source files (full path optional)
SRCS_DIR		=
SRC_FILES		= main.c \
				\
				ft_cd.c ft_echo.c ft_env.c \
				ft_export.c ft_pwd.c ft_unset.c \
				\
				ft_execve.c pipex_stolen.c wrapper.c \
				\
				parser.c tokenizer.c \
				\
				handle_vars.c expand_string.c \
				get_value.c the_matrix.c signals.c\
				\
				print_stuff.c free_space.c cleanup.c \
				ft_realloc.c ft_freentf.c ft_readline.c

SRCS			= $(addprefix $(SRCS_DIR), $(SRC_FILES))

EXE_DIR			= exec/
EXE_BUILT		= 

#folders containing source files [*.c]
VPATH			= src \
				  src/built_ins \
				  src/cmds \
				  src/parser \
				  src/utils \
				  src/vars

# Objects
OBJS_DIR		= obj/
OBJ_FILES		= $(SRCS:.c=.o)
OBJS			= $(addprefix $(OBJS_DIR), $(OBJ_FILES))
GIDEF			=	"""$\
					\#default rules\n$\
					**/.gitignore\n$\
					.git\n$\
					.vscode\n$\
					**/temp\n$\
					**/$(OBJS_DIR)\n$\
					**/resources\n$\
					**/a.out\n$\
					**/*.a\n$\
					**/tester.c\n$\
					\n$\
					\#added rules\n$\
					$(NAME)\n$\
					$(NAME).tar$\
					"""

all: loading $(NAME)
loading:
	@clear
	@i=1; while [ $$i -le 50 ]; do \
		clear; \
		printf "Ɛ"; \
		for j in $$(seq 1 $$i); do printf "="; done; \
		printf "ɔ\n"; \
		sleep 0.025; \
		i=$$((i + 1)); \
	done

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c | $(OBJS_DIR) 
	@$(CC) $(CFLAGS) $(INK) $(DEFS) -c $< -o $@

$(EXE_DIR):
	@mkdir -p $(EXE_DIR)

$(EXE_DIR)%: $(EXE_BUILT)%.c | $(EXE_DIR) $(LIBFT)
	@$(CC) $(CFLAGS) $< $(LIBFT) $(INK) $(DEFS) -o $@

$(LIBFT):
	@echo "${BOLD}creating libft...${RESET}"
	@$(MAKE) -C $(LIBFT_DIR) --quiet

$(NAME): $(LIBFT) $(OBJS)
	@sl -le
	@echo "${BOLD}compiling $(NAME)...${RESET}"
	@$(CC) $(CFLAGS) $(OBJS_DIR)* $(LIBFT) $(LINKS) -o $(NAME) \
	&& echo "${LIGHT_GREEN}DONE${RESET}"

notrainnopainnogain: $(LIBFT) $(OBJS)
	@echo "${BOLD}compiling $(NAME)...${RESET}"
	@$(CC) $(CFLAGS) $(OBJS_DIR)* $(LIBFT) $(LINKS) -o $(NAME) \
	&& echo "${LIGHT_GREEN}DONE${RESET}"

tar: os
	@ls | grep -q "$(NAME).tar" && rm -f $(NAME).tar || true
	@tar -cf $(NAME).tar --exclude=".git" --exclude="$(NAME)" --exclude="$(OBJS_DIR)" ./*

.gitignore:
	@touch .gitignore
	@$(MAKE) .gitignore -C $(LIBFT_DIR) --quiet
	@cat .gitignore | grep -q "#default rules" || \
		echo $(GIDEF)"$$(cat .gitignore)" > .gitignore

show:
	@printf "NAME  		: $(NAME)\n"
	@printf "OS		: $(OS)\n"
	@printf "LIBFT		: $(LIBFT)\n"
	@printf "MINILIBX	: $(MLX)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LINKS		: $(LINKS)\n"
	@printf "INCLUDES	: $(INK)\n"
	@printf "SRCS		: $(SRCS)\n"
	@printf "OBJS		: $(OBJS)\n"

clean:
	@rm -rf $(OBJS_DIR) $(NAME).tar
	@echo "${BOLD}removed:${RESET}\vobjects (.o) and archives (.tar)"

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR) --quiet
	@echo "\texecutable ($(NAME))"

lre: clean all

re: fclean all

.PHONY: os all clean fclean re lre nyaa tar .gitignore show
