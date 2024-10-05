#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <signal.h>

#define VMU_LCD_WIDTH 48
#define VMU_LCD_HEIGHT 32
#define SCALE 10
#define WINDOW_WIDTH (VMU_LCD_WIDTH * SCALE)
#define WINDOW_HEIGHT (VMU_LCD_HEIGHT * SCALE)
#define FONT_PATH "../../vmuPI/fonts/Arial.ttf"
#define EMULATOR_UPDATE_INTERVAL_MS 50 

extern bool keepRunning;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

void intHandler(int dummy);

#endif // COMMON_H
