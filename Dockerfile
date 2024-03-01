# Use a specific version of Debian to ensure reproducibility
FROM debian:bullseye

# Update package lists and install dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    cmake \
    make \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

# The VOLUME directive does not actually sync data between the host and container.
# It's used to mark a directory as holding volumes. Synchronization is handled
# by the Docker run command with the -v or --mount option.
# Consider removing this line if it's not serving a specific purpose.
# VOLUME ["."]

# Build your project
RUN cmake -B ./build/ -S . && make -C ./build/

CMD ["tail", "-f", "/dev/null"]
