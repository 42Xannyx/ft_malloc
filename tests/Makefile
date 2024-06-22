NAME = alloc_tester

INC_PATH = ../src/libft_plus/inc/
LIB_PATH = ..
BUILD_DIR = obj
SRC_DIR = src

# Use wildcard to include all .c files in the src directory
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
LIB_SOURCES = -L$(LIB_PATH) -lft_malloc

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB_SOURCES) -DDEBUG=1
	@echo "Compilation successful"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INC_PATH) -c $< -o $@

clean:
	@make clean -C $(LIB_PATH)
	@rm -rf $(BUILD_DIR)
	@echo "Clean \033[33mok\033[0m"

fclean: clean
	@make fclean -C $(LIB_PATH)
	@rm -f $(NAME)
	@echo "Fclean \033[33mok\033[0m"

re: fclean all

.PHONY: all clean fclean re test
