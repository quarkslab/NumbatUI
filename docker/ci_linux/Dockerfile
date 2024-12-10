FROM ubuntu:latest
SHELL ["/bin/bash", "-c"]

ENV NUMBATUI_WORKING_DIR=/tmp/numbatui

RUN apt-get update 
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
        cmake \
        git \
        build-essential \
        libboost-filesystem-dev libboost-program-options-dev libboost-system-dev libboost-date-time-dev \
        qt6-svg-dev qt6-base-dev qt6-5compat-dev \
        unzip wget \
        libclang-17-dev clang-17 && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR ${NUMBATUI_WORKING_DIR}