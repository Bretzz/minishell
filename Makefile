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
OS				= $(shell uname)

# Libs
LIBFT_DIR		:= libft
LIBFT			= $(LIBFT_DIR)/libft.a
MTX_DIR			:= src/vars/mtxdb

DEFS			=
INK				= -I$(LIBFT_DIR) -I$(MTX_DIR) -I$(CURDIR)
LINKS			= -lreadline #-ltinfo


ifeq ($(OS),Darwin)
	INK			+= -I/usr/local/opt/readline/include
	LINKS		+= -L/usr/local/opt/readline/lib
endif

#source files (full path optional)
SRCS_DIR		=
SRC_FILES		= main.c \
				\
				clean_exit.c handle_line.c init.c \
				unbox_the_line.c \
				\
				ft_cd.c \
				ft_echo.c \
				ft_env.c \
				ft_export.c print_export.c \
				ft_pwd.c \
				ft_unset.c \
				ft_exit.c \
				\
				ft_execve.c pipex_stolen.c builtin_bridge.c \
				execute_pipeline.c execute_command.c \
				exe_cute.c exe_really_cute.c exec_one_of_many.c exec_redir.c \
				here_doc.c here_doc_utils.c \
				safety_first.c \
				\
				parser.c tokenizer.c \
				parser_tiny.c parser_redirectors.c create_redir_out.c \
				token_utils.c print_tokens.c \
				syntax_tokens.c syntax_line.c syntax_checks.c \
				append_line.c stray_docs.c syntax_utils.c \
				free_struct.c token_tiny.c \
				\
				handle_vars.c expand_string.c wide_search.c \
				\
				signals.c initializers.c \
				\
				mtx_addnum.c mtx_addval.c \
				mtx_init.c mtx_free.c \
				mtx_delvar.c mtx_move.c \
				mtx_getindex.c mtx_findval.c \
				mtx_replace.c mtx_vstr_copy.c \
				mtx_setnum.c mtx_setval.c mtx_setdata.c \
				\
				vstr_getname.c vstr_getvalue.c \
				\
				free_space.c cleanup.c cool_stuff.c more_cool_stuff.c weird_strlen.c get_safe_line.c
				
#				ft_readline.c

SRCS			= $(addprefix $(SRCS_DIR), $(SRC_FILES))

B_SRC_FILES		= main_bonus.c \
				\
				bush_is_real.c bush_manage.c bush_pioneers.c \
				handle_line_bonus.c unbox_the_line_bonus.c \
				\
				parser_bonus.c stray_docs_bonus.c \
				syntax_tokens_bonus.c syntax_line_bonus.c \
				tokenizer_bonus.c token_utils_bonus.c \
				\
				execute_command_bonus.c execute_pipeline_bonus.c \
				\
				expand_string_bonus.c get_wild_bonus.c

# Get the corresponding non-bonus version of bonus files (e.g., parser_bonus.c -> parser.c)
B_REPLACED		= $(patsubst %_bonus.c,%.c,$(filter %_bonus.c,$(B_SRC_FILES)))

# Final bonus source list: SRC_FILES minus the ones replaced, plus the bonus files
BONUS_FILES		= $(filter-out $(B_REPLACED), $(SRC_FILES)) $(B_SRC_FILES)

# If you want full paths using SRCS_DIR:
B_SRCS			= $(addprefix $(SRCS_DIR), $(BONUS_FILES))				

HERE_DOCS_DIR	= here_docs

#folders containing source files [*.c]
VPATH			= src \
				  src/built_ins \
				  src/cmds \
				  src/core \
				  src/parser \
				  src/signals \
				  src/utils \
				  src/vars \
				  src/vars/mtxdb

# Objects
OBJS_DIR		= obj/

#Mandatory objects
OBJ_FILES		= $(SRCS:.c=.o)
OBJS			= $(addprefix $(OBJS_DIR), $(OBJ_FILES))

#Bonus objects
B_OBJ_FILES		= $(B_SRCS:.c=.o)
B_OBJS			= $(addprefix $(OBJS_DIR), $(B_OBJ_FILES))

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

all: $(NAME)
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

show_bonus:
#	@printf "BOBJS		: $(B_OBJS)\n"
	@printf "B_REPLACED		  : $(B_REPLACED:.c=.o)\n"
	@printf "MANDATORY REMOVE : $(addprefix $(OBJS_DIR), $(B_SRC_FILES:.c=.o))\n"

$(HERE_DOCS_DIR):
	@mkdir -p $(HERE_DOCS_DIR)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

#-D DEBUG=1				(dopo $(CFLAGS))
$(OBJS_DIR)%.o: $(SRCS_DIR)%.c | $(OBJS_DIR) 
	@$(CC) $(CFLAGS) -D DEBUG=0	$(INK) $(DEFS) -c $< -o $@

$(LIBFT):
	@echo "${BOLD}creating libft...${RESET}"
	@$(MAKE) -C $(LIBFT_DIR) --quiet

$(NAME): $(LIBFT) $(OBJS) $(HERE_DOCS_DIR)
	@sl -le
	@rm -rf $(addprefix $(OBJS_DIR), $(B_SRC_FILES:.c=.o));
	@echo "${BOLD}compiling $(NAME)...${RESET}"
	@$(CC) $(CFLAGS) $(OBJS_DIR)* $(LIBFT) $(LINKS) -o $(NAME) \
	&& echo "${LIGHT_GREEN}DONE${RESET}"

bonus: $(LIBFT) $(B_OBJS) $(HERE_DOCS_DIR)
	@rm -rf $(addprefix $(OBJS_DIR), $(B_REPLACED:.c=.o));
	@echo "${BOLD}compiling $(NAME)_bonus...${RESET}"
	@$(CC) $(CFLAGS) $(OBJS_DIR)* $(LIBFT) $(LINKS) -o $(NAME)_bonus \
	&& echo "${LIGHT_GREEN}DONE${RESET}"

notrainnopainnogain: $(LIBFT) $(OBJS) $(HERE_DOCS_DIR)
	@echo "${BOLD}compiling $(NAME)...${RESET}"
	@$(CC) $(CFLAGS) $(OBJS_DIR)* $(LIBFT) $(LINKS) -o $(NAME) \
	&& echo "${LIGHT_GREEN}DONE${RESET}"

val: notrainnopainnogain
	clear && valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --suppressions=$(CURDIR)/valgrind.supp ./$(NAME)

bval: bonus
	clear && valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --suppressions=$(CURDIR)/valgrind.supp ./$(NAME)_bonus

minival: notrainnopainnogain
	clear && valgrind --track-fds=yes --trace-children=yes --suppressions=$(CURDIR)/valgrind.supp ./$(NAME)

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
#	@printf "SRCS		: $(SRCS)\n"
#	@printf "OBJS		: $(OBJS)\n"
	@printf "BOBJS		: $(B_OBJS)\n"

clean:
	@rm -rf $(OBJS_DIR) $(NAME).tar
	@echo "${BOLD}removed:${RESET}\vobjects (.o) and archives (.tar)"

fclean: clean
	@rm -rf $(NAME) $(NAME)_bonus
	@$(MAKE) fclean -C $(LIBFT_DIR) --quiet
	@echo "\texecutable(s) ($(NAME), $(NAME)_bonus)"

lre: clean all

re: fclean all

.PHONY: os all clean fclean re lre bonus nyaa tar .gitignore show
