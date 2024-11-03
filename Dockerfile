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
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libglu1-mesa-dev \
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
    && cp -r bin/* /usr/local/bin/ \  
    && cp -r share/* /usr/local/share/ \ 
    && cp -r man/* /usr/local/man/ \  
    && cp -r doc/* /usr/local/doc/ \  
    && cd .. && rm -rf cmake-3.30.3-linux-x86_64 cmake-3.30.3-linux-x86_64.tar.gz

# Fetch and build SFML independently before copying source code
RUN git clone --branch 2.6.x --depth 1 https://github.com/SFML/SFML.git /app/SFML \
    && mkdir -p /app/SFML/build \
    && cd /app/SFML/build \
    && cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=/usr/local/ \
    && cmake --build . --target install

# Copy the source code into the container
COPY CMakeLists.txt .
COPY src src
COPY assets assets

# Create a build directory and configure CMake (but do not rebuild SFML)
RUN mkdir -p /app/build
RUN cmake -B build -S . -DCMAKE_PREFIX_PATH=/usr/local/

# Build the project (without rebuilding SFML)
RUN cmake --build build

# Command to run the executable
CMD ["./build/bin/main"]

