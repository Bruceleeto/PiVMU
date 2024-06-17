#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "common.h"

char selectedGame[256] = "";

bool run_select_menu() {
    signal(SIGINT, intHandler);

    TTF_Font* font = TTF_OpenFont(FONT_PATH, 24);
    if (!font) {
        fprintf(stderr, "Unable to load font: %s\n", TTF_GetError());
        return false;
    }

    struct dirent* entry;
    DIR* dp = opendir("../../vmuPI/roms");
    if (dp == NULL) {
        fprintf(stderr, "Unable to open roms directory\n");
        TTF_CloseFont(font);
        return false;
    }

    char roms[100][256];
    int romCount = 0;
    while ((entry = readdir(dp)) != NULL) {
        if (strstr(entry->d_name, ".dci")) {
            strncpy(roms[romCount], entry->d_name, sizeof(roms[romCount]));
            romCount++;
        }
    }
    closedir(dp);

    int selectedIndex = 0;

while (keepRunning && selectedGame[0] == '\0') {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            keepRunning = false;
            break;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP) {
                if (selectedIndex > 0) selectedIndex--;
            } else if (event.key.keysym.sym == SDLK_DOWN) {
                if (selectedIndex < romCount - 1) selectedIndex++;
            } else if (event.key.keysym.sym == SDLK_RETURN) {
                strncpy(selectedGame, roms[selectedIndex], sizeof(selectedGame));
            }
        } else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
                if (selectedIndex > 0) selectedIndex--;
            } else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
                if (selectedIndex < romCount - 1) selectedIndex++;
            } else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
                strncpy(selectedGame, roms[selectedIndex], sizeof(selectedGame));
            }
        }
    }



        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black 
        SDL_RenderClear(renderer);

        for (int i = 0; i < romCount; i++) {
            SDL_Color color = {255, 255, 255};
            SDL_Surface* surface = TTF_RenderText_Solid(font, roms[i], color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            int textW = 0, textH = 0;
            SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
            SDL_Rect textRect = {30, 50 + i * 30, textW, textH};
            SDL_RenderCopy(renderer, texture, NULL, &textRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            if (i == selectedIndex) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White 
                SDL_Rect selectorRect = {10, 50 + i * 30, 10, 10};
                SDL_RenderFillRect(renderer, &selectorRect);
            }
        }

        SDL_RenderPresent(renderer);

      //  SDL_Delay(16); 
    }

    TTF_CloseFont(font);

    return selectedGame[0] != '\0';
}
