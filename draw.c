#include <stdlib.h>

#include "draw.h"
#include "macros.h"

coords *bresenham(coords initial, coords final, SDL_Colour colour) {
    uint16_t dx = abs(final.x - initial.x);
    uint16_t dy = abs(final.y - initial.y);

    coords *line = malloc(sizeof(coords) * MAX(dx, dy));

    uint16_t x = initial.x;
    uint16_t y = initial.y;

    float slope = (float) dy / (float) dx;

    if (slope < 1) {
        int d = 2 * dy - dx;
        int dT = 2 * (dy - dx);
        uint16_t dS = 2 * dy;

        for (int i = 0; x < final.x; ++i) {
            line[i].x = x;
            line[i].y = y;

            line[i].colour = colour;

            if (d < 0)
                d += dS;
            else {
                d += dT;
                ++y;
            }

            x++;
        }
    } else {
        int d = 2 * dx - dy;
        int dT = 2 * (dx - dy);
        uint16_t dS = 2 * dx;

        for (int i = 0; y < final.y; ++i) {

            line[i].x = x;
            line[i].y = y;

            line[i].colour = colour;

            if (d < 0)
                d += dS;
            else {
                d += dT;
                ++x;
            }

            ++y;
        }
    }

    return line;
}

void save_coords(SDL_Colour **canvas, coords *to_save, uint16_t w, uint16_t h, uint16_t side, uint16_t density) {
    for (int i = 0; to_save[i].x < w - side && to_save[i].y < h - side; ++i) {
        for (int x = 0; x < side; ++x)
            for (int y = 0; y < side; ++y)
                canvas[to_save[i].x + x][to_save[i].y + y] = to_save[i].colour;
    }
}

void show_canvas(SDL_Renderer *renderer, SDL_Colour **canvas, uint16_t w, uint16_t h, bool animate) {
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            SDL_SetRenderDrawColor(renderer,
                                   canvas[i][j].r,
                                   canvas[i][j].g,
                                   canvas[i][j].b,
                                   canvas[i][j].a);

            SDL_RenderDrawPoint(renderer, i, j);

            // Refresh animation every 8 pixels drawn
            if (animate && j % 32 == 0)
                SDL_RenderPresent(renderer);
        }
    }

    if (!animate)
        SDL_RenderPresent(renderer);
}

void show_coords(SDL_Renderer *renderer, coords *to_draw, uint16_t lim_w, uint16_t lim_h, uint16_t side, bool animate) {
    for (int i = 0; to_draw[i].x < lim_w && to_draw[i].y < lim_h; ++i) {
        SDL_SetRenderDrawColor(renderer,
                               to_draw[i].colour.r,
                               to_draw[i].colour.g,
                               to_draw[i].colour.b,
                               to_draw[i].colour.a);

        for (int x = 0; x < side; ++x)
            for (int y = 0; y < side; ++y)
                SDL_RenderDrawPoint(renderer, to_draw[i].x * side + x, to_draw[i].y * side + y);

        if (animate)
            SDL_RenderPresent(renderer);
    }

    if (!animate)
        SDL_RenderPresent(renderer);
}