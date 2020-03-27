#ifndef ANTI_ALIASING_ANTIALIASING_H
#define ANTI_ALIASING_ANTIALIASING_H

#include "SDL2/SDL.h"

void super_sample(SDL_Colour **dense, SDL_Colour **canvas, uint16_t w, uint16_t h, uint16_t side, uint16_t density);

void low_pass(SDL_Colour **canvas, uint16_t w, uint16_t h);

#endif //ANTI_ALIASING_ANTIALIASING_H
