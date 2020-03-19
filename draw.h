#ifndef ANTI_ALIASING_DRAW_H
#define ANTI_ALIASING_DRAW_H

#include <stdbool.h>
#include "SDL2/SDL.h"

typedef struct
{
    uint16_t x;
    uint16_t y;
    SDL_Colour colour;
} coords;

coords *bresenham(coords initial, coords final, uint8_t side, bool dense, uint16_t *steps);

void save_coords(SDL_Colour **canvas, coords *to_save, const uint16_t *steps);
void show_canvas(SDL_Renderer *renderer, SDL_Colour **canvas, uint16_t w, uint16_t h, bool animate);
void show_coords(SDL_Renderer *renderer, coords *to_draw, bool animate, const uint16_t *steps);

#endif //ANTI_ALIASING_DRAW_H
