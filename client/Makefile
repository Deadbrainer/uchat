CC = clang
NAME = uchat
INC = inc/*.h
LIBMX = ../libmx/libmx.a
SRC := $(wildcard src/*.c)
OBJ := $(addprefix obj/, $(notdir $(SRC:%.c=%.o)))
OBJ_DIR := obj
FLAGS = -std=c11 -Wno-error=unused-variable -Wall -Wextra -Werror -Wpedantic
DEBUG_FLAGS = -fsanitize=address -fno-omit-frame-pointer
GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`
all: $(NAME)
$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(GTK_LIBS) $(DEBUG_FLAGS) $(OBJ) $(LIBMX) -o ../$(NAME)
	@make clean
	@printf "\r\33[2K$(NAME):\t\t \033[32;1mcreated\033[0m\n"
$(OBJ): | obj
obj:
	@mkdir $@
obj/%.o: src/%.c $(INC)
	@$(CC) $(FLAGS) $(GTK_FLAGS) $(LIBMX) -c $< -o $@
	@printf "\r\33[2K$(NAME):\033[33;1m\t\t compile \033[0m$(<:$(SRC_DIR)/%.c=%)\r"
clean:
	@rm -rf obj/*.o
	@rm -rf ./obj
	@printf "$(NAME):\t\t \033[31;1mdeleted\033[0m $(OBJ_DIR)\r"
uninstall:
	@rm -rf ../$(NAME)
	@printf "$(NAME):\t\t \033[31;1muninstalled\033[0m\n"
