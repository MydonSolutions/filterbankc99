FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get --fix-missing update -y && apt-get install -y \
    git \
    cmake \
    linux-tools-generic \
    pkg-config \
    python3-pip \
&& python3 -m pip install meson ninja pyuvdata

WORKDIR /work

COPY . /work/filterbankc99

RUN cd /work/filterbankc99 \
&& git submodule update --init \
&& meson setup /work/filterbankc99_build -Dhdf5=true \
&& cd /work/filterbankc99_build \
&& ninja install