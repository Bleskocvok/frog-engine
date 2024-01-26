#ifndef NOT_FROG_BUILD_2D

    #pragma once

    #ifdef SDL_HEADER_FOLDER
        #define SDL_HEADER <SDL_HEADER_FOLDER/SDL.h>
    #else
        #define SDL_HEADER <SDL.h>
    #endif

    #ifdef SDL_IMAGE_HEADER_FOLDER
        #define SDL_IMAGE_HEADER <SDL_IMAGE_HEADER_FOLDER/SDL_image.h>
    #else
        #define SDL_IMAGE_HEADER <SDL_image.h>
    #endif

    #ifdef SDL_MIXER_HEADER_FOLDER
        #define SDL_MIXER_HEADER <SDL_MIXER_HEADER_FOLDER/SDL_mixer.h>
    #else
        #define SDL_MIXER_HEADER <SDL_mixer.h>
    #endif

    #ifdef SDL_TTF_HEADER_FOLDER
        #define SDL_TTF_HEADER <SDL_TTF_HEADER_FOLDER/SDL_ttf.h>
    #else
        #define SDL_TTF_HEADER <SDL_ttf.h>
    #endif

    #ifdef SDL_NET_HEADER_FOLDER
        #define SDL_NET_HEADER <SDL_NET_HEADER_FOLDER/SDL_net.h>
    #else
        #define SDL_NET_HEADER <SDL_net.h>
    #endif

#endif
