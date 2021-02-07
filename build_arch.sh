#!/bin/bash
set -eux
docker-compose run --rm --user build arch bash -c '
    SRC_DIR=/src
    BUILD_DIR=/build/aur
    mkdir -p "$SRC_DIR"/artifacts "$(basename "$BUILD_DIR")" && \
        cp -a "$SRC_DIR"/scripts/aur/touche-git "$BUILD_DIR" && \
        mkdir -p "$BUILD_DIR"/src && \
        ln -s "$SRC_DIR" "$BUILD_DIR/src/touche" && \
        cd "$BUILD_DIR" && \
        makepkg -s --noextract --noconfirm && \
        mv *.pkg.tar.* "$SRC_DIR"/artifacts
'
