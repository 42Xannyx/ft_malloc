ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CMAKE_BUILD_DIR := build
LIBFT_MALLOC := $(CMAKE_BUILD_DIR)/libft_malloc_$(HOSTTYPE).so LIBFT_MALLOC_SYMLINK := libft_malloc.so

all: $(CMAKE_BUILD_DIR) $(LIBFT_MALLOC_SYMLINK)

$(CMAKE_BUILD_DIR):
	@cmake -B $(CMAKE_BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
	@cmake --build $(CMAKE_BUILD_DIR)

debug: $(LIBFT_MALLOC_SYMLINK)
	@cmake -B $(CMAKE_BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES -DDEBUG=1 
	@cmake --build $(CMAKE_BUILD_DIR) 

$(LIBFT_MALLOC_SYMLINK): $(LIBFT_MALLOC)
	@ln -s $(LIBFT_MALLOC) $(LIBFT_MALLOC_SYMLINK)

clean:
	@rm -rf $(CMAKE_BUILD_DIR) $(LIBFT_MALLOC_SYMLINK) run_test

test: 
	make -C ./build/
	make -C ./tests/
	./run.sh ./tests/alloc_tester

.PHONY: all clean test
