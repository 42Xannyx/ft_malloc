# ft_malloc

`ft_malloc` is a custom memory allocation library designed to demonstrate dynamic memory management and integration with build systems using CMake.

### Prerequisites

Before you begin, ensure you have installed:

- CMake (version 3.28 or higher recommended)
- GNU Make
- A C compiler (GCC or Clang)

### Setup and Build Instructions

To build the `ft_malloc` project, follow these steps:

1. **Configure the Project with CMake:**

   From the root of the project directory, run:

   ```
   cmake -B ./build/ -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
   ```

   This command configures the project, specifying `./build/` as the directory where the build files will be generated, and `.` indicating the source directory is the current directory.

2. **Build the Project with Make:**

   Next, compile the project using:

   ```
   make -C ./build/
   ```

   This command tells Make to change to the `./build/` directory (`-C ./build/`) and execute the build process.

### Running the Executable

After building, you can run the executable located in the `./build/` directory. The executable is named according to the host system's type and architecture, following the pattern `ft_malloc-${HOSTTYPE}`. Replace `${HOSTTYPE}` with the appropriate value for your system, which is typically a combination of your machine's architecture and operating system.

For example, if your `HOSTTYPE` is `x86_64_Linux`, the executable name would be `ft_malloc-x86_64_Linux`. To run the executable, use:

```
./build/ft_malloc-${HOSTTYPE}
```

Replace `ft_malloc-${HOSTTYPE}` with the actual name of your built executable.

### Additional Information

### Docker Setup

To facilitate development and testing within a Docker container, follow these steps to build your Docker image and run your container with real-time synchronization between your host machine and the Docker environment.

#### Building the Docker Image

First, build your Docker image from the root of your project directory:

```sh
docker build -t malloc_container .
```

This command builds the Docker image with all necessary dependencies installed, as specified in your `Dockerfile`, and tags it as `malloc_container`.

#### Running the Docker Container

To run your Docker container and mount your current project directory . (`pwd`) to `/app` in the container for file synchronization, use:

```sh
docker run -it --name DebianMalloc -v .:/app malloc_container /bin/bash
```

This command starts the container with an interactive shell, allowing you to execute commands within the container immediately. Replace `/bin/bash` with `/bin/sh` if your base image does not include Bash.

For more details on configuring and customizing the build process, refer to the CMake and GNU Make documentation. Additionally, the Docker documentation provides extensive information on container management, image creation, and best practices for using Docker in development environments.
