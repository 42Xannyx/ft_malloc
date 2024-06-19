cmake_minimum_required(VERSION 3.18)

project(ft_malloc)
set(CMAKE_C_STANDARD 23)

option(DEBUG "Build with debug information" OFF)
file(GLOB_RECURSE SOURCES "src/test/*.c")

file(GLOB_RECURSE LIBFT_PLUS_SOURCES "src/libft_plus/*.c")
add_library(libft_plus STATIC ${LIBFT_PLUS_SOURCES})

# Check if HOSTTYPE is set in the environment, else construct it
if(NOT DEFINED ENV{HOSTTYPE})
    execute_process(COMMAND uname -m OUTPUT_VARIABLE UNAME_M OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND uname -s OUTPUT_VARIABLE UNAME_S OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(HOSTTYPE "${UNAME_M}_${UNAME_S}")
else()
    set(HOSTTYPE $ENV{HOSTTYPE})
endif()

if(DEBUG)
    message(STATUS "Building in debug mode")
    add_definitions(-DDEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_C_FLAGS "-g3 -fsanitize=address -Wall -Wextra -Werror -Wunused-result -Wunused-variable")
else()
    message(STATUS "Building in release mode")
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_C_FLAGS "-Ofast -Wall -Wextra -Werror -Wunused-result -Wunused-variable")
endif()

add_definitions(-DHOSTTYPE="${HOSTTYPE}")
set(EXECUTABLE_NAME "ft_malloc-${HOSTTYPE}")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

include_directories(${PROJECT_SOURCE_DIR}/lib/libft ${PROJECT_SOURCE_DIR}/src/libft_plus)

add_custom_target(
    BuildLibft ALL
    COMMAND make
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/libft
    COMMENT "Compiling libft library"
)

add_library(libft STATIC IMPORTED)
set_property(TARGET libft PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libft/libft.a)

add_dependencies(${EXECUTABLE_NAME} BuildLibft libft_plus)

target_link_libraries(${EXECUTABLE_NAME} libft libft_plus)
include_directories(${PROJECT_SOURCE_DIR}/lib/libft/INC/ ${PROJECT_SOURCE_DIR}/src/libft_plus/inc/)

if(DEBUG)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=address)
endif()

# Print out the compile options for verification
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
