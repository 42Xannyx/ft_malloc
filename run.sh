#!/bin/bash

OS=$(uname)

if [ "$OS" = 'Linux' ]; then
	export LD_LIBRARY_PATH=$PWD
	export LD_PRELOAD=$PWD/libft_malloc.so
elif [ "$OS" = "Darwin" ]; then
	export DYLD_LIBRARY_PATH=$PWD
	export DYLD_INSERT_LIBRARIES=$PWD/libft_malloc.so
	export DYLD_FORCE_FLAT_NAMESPACE=1
fi

$@

