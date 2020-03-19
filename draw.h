#ifndef ANTI_ALIASING_DRAW_H
#define ANTI_ALIASING_DRAW_H

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

#include "demo.h"

#define MAX(a, b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

typedef struct
{
    uint16_t x;
    uint16_t y;
    SDL_Colour colour;
} coords;

coords *bresenham(coords initial, coords final, uint16_t *steps);
coords *get_dense_line(coords initial, coords final, SDL_Colour **canvas, uint8_t side, uint16_t *steps);

void draw_coords(Demo *running_demo, coords *to_draw, SDL_Colour **canvas, const uint16_t *steps);
void read_and_draw_line(Demo *running_demo, SDL_Colour **canvas, FILE *in);

#endif //ANTI_ALIASING_DRAW_H
