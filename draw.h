#ifndef ANTI_ALIASING_DRAW_H
#define ANTI_ALIASING_DRAW_H

#include <stdbool.h>
#include "SDL2/SDL.h"

typedef struct coords
{
    uint16_t x;
    uint16_t y;
} coords;

SDL_Colour **
bresenham(coords initial, coords final, uint16_t side, uint16_t density, SDL_Colour **canvas, SDL_Colour colour);

void show_canvas(SDL_Renderer *renderer, SDL_Colour **canvas, uint16_t w, uint16_t h, uint16_t scale, bool animate);

#endif //ANTI_ALIASING_DRAW_H
