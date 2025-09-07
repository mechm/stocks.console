# Stage 1: The builder stage. This image is temporary and will not be deployed.
# Use a modern base image for compiling software
FROM ubuntu:22.04 AS builder

# Install necessary tools for downloading and compiling
# Add 'ca-certificates' to fix the SSL verification error
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    wget \
    build-essential \
    libssl-dev \
    libcurl4-openssl-dev \
    zlib1g-dev \
    ca-certificates && rm -rf /var/lib/apt/lists/*

# The rest of your Dockerfile goes here.
# For example:
# Set a variable for the latest CMake version from its release page
ENV CMAKE_VERSION "3.29.3"

# Download and extract the latest CMake source
WORKDIR /usr/src
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz && \
    tar -xzf cmake-${CMAKE_VERSION}.tar.gz && \
    rm cmake-${CMAKE_VERSION}.tar.gz

# Configure and build CMake
WORKDIR /usr/src/cmake-${CMAKE_VERSION}
RUN ./bootstrap --prefix=/usr/local --parallel=$(nproc) && \
    make -j$(nproc) && \
    make install

# Make sure to set the PATH to include the new CMake installation
ENV PATH="/usr/local/bin:${PATH}"

# Now, build your C++ application
WORKDIR /app
COPY /stocks.console/src/stocks.console.cpp CMakeLists.txt ./

# Run the build process for your application
RUN cmake . && make

# -----------------------------------------------------------------------------

# Stage 2: The final production stage. This image is lightweight and contains only the executable.
# Use a clean, minimal base image
FROM ubuntu:22.04

# Set the working directory for the final image
WORKDIR /opt/stock_console

# Copy the compiled executable from the 'builder' stage
# The '--from=builder' flag is the key instruction here
COPY --from=builder /app/stocks.console ./

# Set the command to run the executable
CMD ["./stocks.console"]