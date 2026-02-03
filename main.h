#ifndef MAIN_H
#define MAIN_H

#include "computer.h"
#include <SDL2/SDL.h>

#define HEIGHT 256
#define WIDTH 512

uint8_t scancodeToAscii(SDL_KeyboardEvent *key);
void displayLoop(SDL_Window *window, SDL_Renderer *rend, SDL_Texture *texture, uint32_t *pixels, struct HackComputer *computer);
void parsePixels(struct HackComputer *computer, uint32_t *pixels);
SDL_Window *initDisplay();
SDL_Renderer *initRender(SDL_Window *wind);

#endif
