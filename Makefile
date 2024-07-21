ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CMAKE_BUILD_DIR := build
LIBFT_MALLOC := libft_malloc_$(HOSTTYPE).so 
LIBFT_MALLOC_SYMLINK := libft_malloc.so
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := inc
CFLAGS := -Wall -Wextra -Werror -pedantic -fPIC
FLAGS_LIB := -shared
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT_MALLOC)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(LIBFT_MALLOC): $(OBJS)
	$(CC) $(FLAGS_LIB) -o $@ $(OBJS)
	@echo "$(LIBFT_MALLOC) compiled successfully"
	@ln -sf $(LIBFT_MALLOC) $(LIBFT_MALLOC_SYMLINK)

clean:
	rm -rf $(OBJ_DIR)
	@echo "Object files cleaned"

fclean: clean
	rm -f $(LIBFT_MALLOC) $(LIBFT_MALLOC_SYMLINK)
	@echo "$(LIBFT_MALLOC) and $(LIBFT_MALLOC_SYMLINK) removed"

debug: CFLAGS += -g3 -DDEBUG=1 
debug: all

re: fclean all

test: CFLAGS += -g3 -DTEST=1
test: $(LIBFT_MALLOC)
	make -C ./tests/
	./run.sh ./tests/alloc_tester

.PHONY: all clean fclean re debug test
