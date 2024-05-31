cmake_minimum_required(VERSION 3.18)

project(ft_malloc)
set(CMAKE_C_STANDARD 23)

option(DEBUG "Build with debug information" OFF)

# Set CMake to compile all files in the src directory except those in libft_plus
file(GLOB_RECURSE SOURCES "src/test/*.c")

# Compile files in libft_plus directory as a separate library
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
    set(CMAKE_BUILD_TYPE Debug)
    set(COMMON_COMPILE_OPTIONS "-g3" "-fsanitize=address")
else()
    message(STATUS "Building in release mode")
    set(CMAKE_BUILD_TYPE Release)
    set(COMMON_COMPILE_OPTIONS "")
endif()

# Add the HOSTTYPE definition for the preprocessor
add_definitions(-DHOSTTYPE="${HOSTTYPE}")

# Construct the executable name based on HOSTTYPE
set(EXECUTABLE_NAME "ft_malloc-${HOSTTYPE}")

# Add the executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Include directories for the header files of both libft and libft_plus
include_directories(${PROJECT_SOURCE_DIR}/lib/libft ${PROJECT_SOURCE_DIR}/src/libft_plus)

# Add custom command to build the libft library before the main project
add_custom_target(
    BuildLibft ALL
    COMMAND make
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/libft
    COMMENT "Compiling libft library"
)

# Add the libft.a static library
add_library(libft STATIC IMPORTED)
set_property(TARGET libft PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libft/libft.a)

# Make sure libft and libft_plus are built before compiling the main project
add_dependencies(${EXECUTABLE_NAME} BuildLibft libft_plus)

# Link the executable with the libft and libft_plus libraries
target_link_libraries(${EXECUTABLE_NAME} libft libft_plus)
include_directories(${PROJECT_SOURCE_DIR}/lib/libft/INC/ ${PROJECT_SOURCE_DIR}/src/libft_plus/inc/)

# Set compile options for the executable
target_compile_options(${EXECUTABLE_NAME} PRIVATE ${COMMON_COMPILE_OPTIONS})

# Set link options for the executable
if(DEBUG)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=address)
endif()
