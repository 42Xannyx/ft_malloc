cmake_minimum_required(VERSION 3.18)

# Set the project name
project(ft_malloc)

# Set CMake to compile all files in the src directory
file(GLOB SOURCES "src/**/*.c")

# Check if HOSTTYPE is set in the environment, else construct it
if(NOT DEFINED ENV{HOSTTYPE})
    execute_process(COMMAND uname -m OUTPUT_VARIABLE UNAME_M OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND uname -s OUTPUT_VARIABLE UNAME_S OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(HOSTTYPE "${UNAME_M}_${UNAME_S}")
else()
    set(HOSTTYPE $ENV{HOSTTYPE})
endif()

# Add the HOSTTYPE definition for the preprocessor
add_definitions(-DHOSTTYPE="${HOSTTYPE}")

# Construct the executable name based on HOSTTYPE
set(EXECUTABLE_NAME "ft_malloc-${HOSTTYPE}")

# Add the executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Include directories for the header files
include_directories(${PROJECT_SOURCE_DIR}/lib/libft)

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

# Make sure libft is built before compiling the main project
add_dependencies(${EXECUTABLE_NAME} BuildLibft)

# Link the executable with the libft library
target_link_libraries(${EXECUTABLE_NAME} libft)
