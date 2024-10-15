# Specify the base image
FROM ubuntu:latest
# Set the working directory inside the container
WORKDIR /app

# Install dependencies, including tools to build cmake from source
RUN apt-get update && apt-get install -y \
    g++ \
    build-essential \
    wget \
    git \
    libsfml-dev \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libdrm-dev \
    libgbm-dev \
    libfreetype6-dev \ 
    libx11-dev \ 
    libogg-dev \
    # Clean up apt cache to reduce image size
    && rm -rf /var/lib/apt/lists/*  

# Download and install a specific version of CMake (3.30.3)
RUN wget https://github.com/Kitware/CMake/releases/download/v3.30.3/cmake-3.30.3-linux-x86_64.tar.gz \
     && tar -xzvf cmake-3.30.3-linux-x86_64.tar.gz \
    && cd cmake-3.30.3-linux-x86_64 \
    # Copy directories to /usr/local/dir
    && cp -r bin/* /usr/local/bin/ \  
    && cp -r share/* /usr/local/share/ \ 
    && cp -r man/* /usr/local/man/ \  
    && cp -r doc/* /usr/local/doc/ \  
    && cd .. && rm -rf cmake-3.30.3-linux-x86_64 cmake-3.30.3-linux-x86_64.tar.gz

# Copy source code into the container
COPY CMakeLists.txt .
COPY src src

# Create a build directory and run cmake to configure and build your project
RUN cmake -B build \
    && cmake --build build

# Command to run the executable
CMD ["./build/bin/main"]

