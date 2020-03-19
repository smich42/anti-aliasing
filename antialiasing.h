#ifndef ANTI_ALIASING_ANTIALIASING_H
#define ANTI_ALIASING_ANTIALIASING_H

#include "SDL2/SDL.h"

void SS(SDL_Colour **canvas, uint8_t div, uint16_t w, uint16_t h);
void low_pass(SDL_Colour **canvas, uint16_t w, uint16_t h);

#endif //ANTI_ALIASING_ANTIALIASING_H
