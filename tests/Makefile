NAME = alloc_tester
INC_PATH = ../src/libft_plus/inc/
LIB_PATH = ..
BUILD_DIR = obj
SRC_DIR = src
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
LIB_NAME = ft_malloc
LIB_FILE = $(LIB_PATH)/lib$(LIB_NAME).so
CFLAGS = -Wall -Wextra -Werror

CFLAGS += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address

all: $(NAME)

$(NAME): $(LIB_FILE) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -L$(LIB_PATH) -l$(LIB_NAME) $(LDFLAGS)
	@echo "Compilation successful"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INC_PATH) -c $< -o $@

$(LIB_FILE):
	@$(MAKE) -C $(LIB_PATH) DEBUG=$(DEBUG)

clean:
	@$(MAKE) clean -C $(LIB_PATH)
	@rm -rf $(BUILD_DIR)
	@echo "Clean \033[33mok\033[0m"

fclean: clean
	@$(MAKE) fclean -C $(LIB_PATH)
	@rm -f $(NAME)
	@echo "Fclean \033[33mok\033[0m"

re: fclean all

.PHONY: all clean fclean re test
