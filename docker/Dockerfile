FROM debian:trixie

# SHELL ["/bin/bash", "-c"]

ENV USER="docker" \
    HOME="/home/docker"

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get install -y \
        cmake \
        git \
        build-essential \
        libboost-filesystem-dev libboost-program-options-dev libboost-system-dev libboost-date-time-dev \
        qt6-svg-dev qt6-base-dev qt6-5compat-dev \
        unzip wget \
        libx11-xcb1 libxrender1 libxkbcommon-x11-0 \
        libc++-19-dev liblld-19-dev llvm-19-dev libclang-19-dev clang-19 && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN adduser --disabled-password --gecos '' --home "$HOME" "$USER"
USER $USER

WORKDIR $HOME

RUN git clone https://github.com/quarkslab/NumbatUI.git

WORKDIR $HOME/NumbatUI

RUN cmake -B build \
          -DCMAKE_BUILD_TYPE="Release" \
          #   -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static-libstdc++" \
          -DBUILD_CXX_LANGUAGE_PACKAGE=OFF \
          -DBUILD_PYTHON_LANGUAGE_PACKAGE=OFF

RUN cmake --build build --target NumbatUI -j $(nproc)


ENTRYPOINT [ "/home/docker/NumbatUI/build/app/NumbatUI" ]
