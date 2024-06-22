ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CMAKE_BUILD_DIR := build
LIBFT_MALLOC := $(CMAKE_BUILD_DIR)/libft_malloc_$(HOSTTYPE).so
LIBFT_MALLOC_SYMLINK := libft_malloc.so

all: $(CMAKE_BUILD_DIR) $(LIBFT_MALLOC_SYMLINK)

$(CMAKE_BUILD_DIR):
	@cmake -B $(CMAKE_BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Release
	@cmake --build $(CMAKE_BUILD_DIR)

debug:
	@cmake -B $(CMAKE_BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Debug -DDEBUG=1
	@cmake --build $(CMAKE_BUILD_DIR)

clean:
	@rm -rf $(CMAKE_BUILD_DIR) $(LIBFT_MALLOC_SYMLINK) run_test

test: run_test
	@$(CC) $(CFLAGS) -o run_test ./src/test/test.c -I./src/libft_plus/inc/
	@./run.sh ./run_test

run_test: ./src/test/test.c $(LIBFT_MALLOC_SYMLINK)

.PHONY: all clean test
