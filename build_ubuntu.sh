#!/bin/bash
set -eux
docker-compose run --rm --user `id -u`:`id -g` ubuntu bash -c '
    BUILD_DIR=/tmp/build-ubuntu
    cmake -B "$BUILD_DIR" -S . -DCMAKE_INSTALL_PREFIX=/usr && \
        make -C "$BUILD_DIR" -j2 && \
        make -C "$BUILD_DIR" package && \
        mkdir -p artifacts && \
        mv "$BUILD_DIR"/*.deb artifacts/
'
