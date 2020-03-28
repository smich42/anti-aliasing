#ifndef ANTI_ALIASING_DRAW_H
#define ANTI_ALIASING_DRAW_H

#include <stdbool.h>
#include "SDL2/SDL.h"
#include "demo.h"

typedef struct coords
{
    uint16_t x;
    uint16_t y;
} coords;

void bresenham(coords initial, coords final, uint16_t side, uint16_t density, Canvas *canvas, SDL_Colour colour);

void enlarge_canvas(Canvas *canvas, uint16_t scale);

void show_canvas(SDL_Renderer *renderer, Canvas *canvas, uint16_t scale, bool animate);

#endif //ANTI_ALIASING_DRAW_H
