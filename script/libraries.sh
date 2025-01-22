#!/bin/sh

set -e

SDL_V="2.28.5"
SDL_NET_V="2.2.0"
SDL_TTF_V="2.20.2"
SDL_IMAGE_V="2.8.1"
SDL_MIXER_V="2.6.3"

DST="$(realpath "$(dirname "$0")/../engine/libraries/")"

WGET_FLAGS=""
UNZIP_FLAGS=""

while getopts "q" p; do
    case "$p" in
        q)
            WGET_FLAGS="-nv"
            UNZIP_FLAGS="-q"
            ;;
    esac
done

TEMP_DIR=$(mktemp --tmpdir -d XXXXXXXXXXX)
(
    cd "$TEMP_DIR"

    wget $WGET_FLAGS "https://github.com/libsdl-org/SDL/releases/download/release-${SDL_V}/SDL2-${SDL_V}.zip"
    wget $WGET_FLAGS "https://github.com/libsdl-org/SDL_net/releases/download/release-${SDL_NET_V}/SDL2_net-${SDL_NET_V}.zip"
    wget $WGET_FLAGS "https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL_TTF_V}/SDL2_ttf-${SDL_TTF_V}.zip"
    wget $WGET_FLAGS "https://github.com/libsdl-org/SDL_image/releases/download/release-${SDL_IMAGE_V}/SDL2_image-${SDL_IMAGE_V}.zip"
    wget $WGET_FLAGS "https://github.com/libsdl-org/SDL_mixer/releases/download/release-${SDL_MIXER_V}/SDL2_mixer-${SDL_MIXER_V}.zip"

    unzip $UNZIP_FLAGS "SDL2-${SDL_V}.zip"               -d "$DST"
    unzip $UNZIP_FLAGS "SDL2_net-${SDL_NET_V}.zip"       -d "$DST"
    unzip $UNZIP_FLAGS "SDL2_ttf-${SDL_TTF_V}.zip"       -d "$DST"
    unzip $UNZIP_FLAGS "SDL2_image-${SDL_IMAGE_V}.zip"   -d "$DST"
    unzip $UNZIP_FLAGS "SDL2_mixer-${SDL_MIXER_V}.zip"   -d "$DST"
)

rm -rf "$TEMP_DIR"
