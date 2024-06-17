#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <signal.h>
#include "common.h"

bool startEmulator = false;

bool run_select_menu(); 

bool run_start_menu() {
    signal(SIGINT, intHandler);

    SDL_Surface* imageSurface = IMG_Load("../../vmuPI/images/pi.png");
    if (!imageSurface) {
        fprintf(stderr, "Unable to load image: %s\n", IMG_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    if (!texture) {
        fprintf(stderr, "Unable to create texture: %s\n", SDL_GetError());
        return false;
    }

    uint32_t startTime = SDL_GetTicks();

    while (keepRunning && !startEmulator) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keepRunning = false;
                break;
            }
        }

        if (SDL_GetTicks() - startTime > 5000) {
            startEmulator = true;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); 
    }

    SDL_DestroyTexture(texture);

    return startEmulator;
}
