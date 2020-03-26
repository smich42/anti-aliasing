#ifndef ANTI_ALIASING_DRAW_H
#define ANTI_ALIASING_DRAW_H

#include <stdbool.h>
#include "SDL2/SDL.h"

typedef struct coords
{
    uint16_t x;
    uint16_t y;
    SDL_Colour colour;
} coords;

coords *bresenham(coords initial, coords final, SDL_Colour colour);

void save_coords(SDL_Colour **canvas, coords *to_save, uint16_t w, uint16_t h, uint16_t side, uint16_t density);

void show_canvas(SDL_Renderer *renderer, SDL_Colour **canvas, uint16_t w, uint16_t h, bool animate);
void show_coords(SDL_Renderer *renderer, coords *to_draw, uint16_t lim_w, uint16_t lim_h, uint16_t side, bool animate);

#endif //ANTI_ALIASING_DRAW_H
