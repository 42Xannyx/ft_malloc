FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    cmake \
    make \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN make

CMD ["tail", "-f", "/dev/null"]
