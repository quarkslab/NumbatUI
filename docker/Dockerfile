FROM debian:trixie-slim

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
    apt-get autoremove && \
    rm -rf /var/lib/apt/lists/*

RUN adduser --disabled-password --gecos '' --home "$HOME" "$USER"
USER $USER

WORKDIR $HOME

# build app and only keep built binaries
RUN git clone https://github.com/quarkslab/NumbatUI.git NumbatUI-build \
    && ( cd NumbatUI-build \  
         && cmake -B build \
                  -DCMAKE_BUILD_TYPE="Release" \
                  -DBUILD_CXX_LANGUAGE_PACKAGE=OFF \
                  -DBUILD_PYTHON_LANGUAGE_PACKAGE=OFF \
         && cmake --build build --target NumbatUI -j $(nproc) )\
    && mkdir NumbatUI \
    && mv NumbatUI-build/build/app/NumbatUI NumbatUI/ \
    && mv -f NumbatUI-build/bin/app/* NumbatUI/ \
    && rm -rf NumbatUI-build

ENTRYPOINT [ "/home/docker/NumbatUI/NumbatUI" ]
