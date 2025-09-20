## Stage 1: The vcpkg setup stage.
#FROM ubuntu:24.04 AS vcpkg_stage
#
## Install necessary tools for vcpkg and its dependencies
#RUN apt-get update && \
#    apt-get install -y --no-install-recommends \
#    git \
#    build-essential \
#    cmake \
#    curl \
#    unzip \
#    tar \
#    zip \
#    ca-certificates \
#    && rm -rf /var/lib/apt/lists/*
#
## Clone vcpkg to a known directory
#WORKDIR /vcpkg
#
#RUN git clone https://github.com/microsoft/vcpkg.git . && \
#    ./bootstrap-vcpkg.sh
#
#RUN git --version
#
## ------------------------------------------------------------------------------
#
## Stage 2: The application builder stage.
#FROM ubuntu:24.04 AS builder
#
#COPY --from=vcpkg_stage /vcpkg /vcpkg
#ENV VCPKG_ROOT="/vcpkg"
#
#WORKDIR /app
#
#RUN ls -l
## Copy all source files, CMakeLists.txt, vcpkg.json, and vcpkg-configuration.json
#COPY stocks.console/ ./
#COPY vcpkg.json ./vcpkg.json
#COPY vcpkg-configuration.json ./vcpkg-configuration.json
##RUN git --version
#RUN ls -l
##RUN ls -l /app/stocks.console
#RUN cat /app/vcpkg.json
#
#
## Install dependencies from vcpkg.json (manifest mode)
##RUN ${VCPKG_ROOT}/vcpkg install
#
##RUN apt-get update && apt-get install -y git
#
## Install necessary tools for vcpkg and its dependencies
##RUN apt-get update && \
##    apt-get install -y --no-install-recommends \
##    git \
##    build-essential \
##    cmake \
##    curl \
##    unzip \
##    tar \
##    zip \
##    ca-certificates \
##    && rm -rf /var/lib/apt/lists/*
#
#WORKDIR /app/stocks.console
#
#
#RUN apt-get update && \
#    apt-get install -y --no-install-recommends \
#    git \
#    build-essential \
#    cmake \
#    curl \
#    unzip \
#    tar \
#    zip \
#    ca-certificates \
#    && rm -rf /var/lib/apt/lists/*
#
#
#RUN ${VCPKG_ROOT}/vcpkg install --triplet=x64-linux && \
#    cmake . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake && \
#    make
## ------------------------------------------------------------------------------
#
## Stage 3: The final production stage. This image is lightweight.
#FROM ubuntu:22.04
#
## Set the working directory for the final image
#WORKDIR /opt/stock_console
#
## Copy the compiled executable from the 'builder' stage
#COPY --from=builder /app/stocks.console/stocks.console ./stocks.console
#
## Set the command to run the executable
#CMD ["./stocks.console"]


# Use the official postgres image as the base
FROM postgres:13

# Copy a custom SQL script to the container's entrypoint directory
# This script will be executed automatically when the container starts
COPY ./db/init.sql /docker-entrypoint-initdb.d/