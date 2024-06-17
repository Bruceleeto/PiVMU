#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <evmu/hw/evmu_device.h>
#include "common.h"

bool keepRunning = true;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_GameController* gameController = NULL;

typedef struct {
    float phase;
    float frequency;
    bool active;
} BuzzerState;

BuzzerState buzzerState = {0, 440.0f, false};

void intHandler(int dummy) {
    keepRunning = false;
}

void audioCallback(void* userdata, Uint8* stream, int len) {
    BuzzerState* state = (BuzzerState*)userdata;
    float* fstream = (float*)stream;
    int samples = len / sizeof(float);

    if (state->active) {
        for (int i = 0; i < samples; ++i) {
            fstream[i] = 0.5f * sinf(2.0f * M_PI * state->frequency * state->phase);
            state->phase += 1.0f / 44100.0f;
            if (state->phase >= 1.0f) {
                state->phase -= 1.0f;
            }
        }
    } else {
        memset(stream, 0, len);
    }
}

void onScreenRefresh_(EvmuLcd* pLcd) {
    void** userdata = GblBox_userdata(GBL_BOX(pLcd));
    SDL_Texture* texture = (SDL_Texture*)userdata[1];

    Uint32 pixels[VMU_LCD_WIDTH * VMU_LCD_HEIGHT];
    for (unsigned h = 0; h < VMU_LCD_HEIGHT; ++h) {
        for (unsigned w = 0; w < VMU_LCD_WIDTH; ++w) {
            bool pixel = EvmuLcd_pixel(pLcd, w, h);
            Uint8 color = pixel ? 255 : 0;
            pixels[h * VMU_LCD_WIDTH + w] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), color, color, color, 255);
        }
    }

    SDL_UpdateTexture(texture, NULL, pixels, VMU_LCD_WIDTH * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    pLcd->screenChanged = GBL_FALSE;
}

void toneStart(EvmuBuzzer* buzzer, int frequency) {
    buzzerState.frequency = (float)frequency;
    buzzerState.active = true;
}

void toneStop(EvmuBuzzer* buzzer) {
    buzzerState.active = false;
}

extern bool run_start_menu();
extern bool run_select_menu();
extern char selectedGame[256];



int main(int argc, char* argv[]) {
    signal(SIGINT, intHandler);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Dreamcast VMU Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (SDL_NumJoysticks() > 0) {
        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                gameController = SDL_GameControllerOpen(i);
                if (gameController) {
                    break;
                }
            }
        }
    }

    if (!run_start_menu() || !run_select_menu()) {
        return -1;
    }

    SDL_AudioSpec desiredSpec, obtainedSpec;
    SDL_zero(desiredSpec);
    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_F32SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 1024;
    desiredSpec.callback = audioCallback;
    desiredSpec.userdata = &buzzerState;

    if (SDL_OpenAudio(&desiredSpec, &obtainedSpec) < 0) {
        fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
        return -1;
    }

    SDL_PauseAudio(0);

    EvmuDevice* pDevice = EvmuDevice_create();
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, VMU_LCD_WIDTH, VMU_LCD_HEIGHT);
    SDL_ShowCursor(SDL_DISABLE);

    void** userdata = malloc(2 * sizeof(void*));
    userdata[0] = renderer;
    userdata[1] = texture;
    GblBox_setUserdata(GBL_BOX(pDevice->pLcd), userdata);

    GBL_CONNECT(pDevice->pLcd, "screenRefresh", onScreenRefresh_);
    GBL_CONNECT(pDevice->pBuzzer, "toneStart", toneStart);
    GBL_CONNECT(pDevice->pBuzzer, "toneStop", toneStop);

    char romPath[512];
    snprintf(romPath, sizeof(romPath), "../../vmuPI/roms/%s", selectedGame);
    EvmuFileManager_load(pDevice->pFileMgr, romPath);

    EvmuGamepad* pGamepad = EVMU_GAMEPAD(pDevice->pGamepad);

    uint64_t lastUpdateTime = SDL_GetTicks();
    while (keepRunning) {
        SDL_Event event;
 while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
        keepRunning = false;
        break;
    }

    bool isPressed = false;
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        isPressed = (event.type == SDL_KEYDOWN);
        switch (event.key.keysym.sym) {
            case SDLK_UP: pGamepad->up = isPressed; break;
            case SDLK_DOWN: pGamepad->down = isPressed; break;
            case SDLK_LEFT: pGamepad->left = isPressed; break;
            case SDLK_RIGHT: pGamepad->right = isPressed; break;
            case SDLK_a: pGamepad->a = isPressed; break;
            case SDLK_b: pGamepad->b = isPressed; break;
            default: break;
        }
    } else if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
        isPressed = (event.type == SDL_CONTROLLERBUTTONDOWN);
        switch (event.cbutton.button) {
            case SDL_CONTROLLER_BUTTON_DPAD_UP: pGamepad->up = isPressed; break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN: pGamepad->down = isPressed; break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT: pGamepad->left = isPressed; break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: pGamepad->right = isPressed; break;
            case SDL_CONTROLLER_BUTTON_A: pGamepad->a = isPressed; break;
            case SDL_CONTROLLER_BUTTON_B: pGamepad->b = isPressed; break;
            case SDL_CONTROLLER_BUTTON_START: break; // to do and some quit / return to main menu shit
            default: break;
        }
    }
}

        uint64_t currentTicks = SDL_GetTicks();
        uint64_t deltaTicks = currentTicks - lastUpdateTime;

        if (deltaTicks >= EMULATOR_UPDATE_INTERVAL_MS) {
            EvmuIBehavior_update(EVMU_IBEHAVIOR(pDevice), deltaTicks * 1000000);
            lastUpdateTime = currentTicks;
        }

        onScreenRefresh_(pDevice->pLcd);
       // SDL_Delay(20);
    }

    if (gameController) {
        SDL_GameControllerClose(gameController);
    }
    free(userdata);
    EvmuDevice_unref(pDevice);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
