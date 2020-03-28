#include <stdio.h>
#include <stdlib.h>

#include "draw.h"
#include "demo.h"

void bresenham(coords initial, coords final, uint16_t side, uint16_t density, Canvas *canvas, SDL_Colour colour)
{
    uint16_t inc = side / density;
    uint16_t dx = abs(final.x - initial.x);
    uint16_t dy = abs(final.y - initial.y);

    uint16_t x = initial.x;
    uint16_t y = initial.y;

    float slope = (float) dy / (float) dx;

    if (slope < 1)
    {
        int d = (2 * dy) - dx;
        int dT = 2 * (dy - dx);
        uint16_t dS = 2 * dy;

        for (int i = 0; x < final.x; ++i)
        {
            for (int add_x = 0; add_x < side && x + add_x < final.x; ++add_x)
                for (int add_y = 0; add_y < side && y + add_y < final.y; ++add_y)
                    canvas->data[x + add_x][y + add_y] = colour;

            if (d < 0)
                d += dS;
            else
            {
                d += dT;
                y += inc;
            }

            x += inc;
        }
    }
    else
    {
        int d = (2 * dx) - dy;
        int dT = 2 * (dx - dy);
        uint16_t dS = 2 * dx;

        for (int i = 0; y < final.y; ++i)
        {
            for (int add_x = 0; add_x < side && x + add_x < final.x; ++add_x)
                for (int add_y = 0; add_y < side && y + add_y < final.y; ++add_y)
                    canvas->data[x + add_x][y + add_y] = colour;

            if (d < 0)
                d += dS;
            else
            {
                d += dT;
                x += inc;
            }

            y += inc;
        }
    }
}

void enlarge_canvas(Canvas *canvas, uint16_t scale)
{

}

void show_canvas(SDL_Renderer *renderer, Canvas *canvas, uint16_t scale, bool animate)
{
    for (int i = 0; i < canvas->w; ++i)
    {
        for (int j = 0; j < canvas->h; ++j)
        {
            SDL_SetRenderDrawColor(renderer,
                                   canvas->data[i][j].r,
                                   canvas->data[i][j].g,
                                   canvas->data[i][j].b,
                                   canvas->data[i][j].a);

            for (int x = 0; x < scale; ++x)
                for (int y = 0; y < scale; ++y)
                    SDL_RenderDrawPoint(renderer, i * scale + x, j * scale + y);

            // Refresh animation every 100 pixels drawn
            if (animate && j % 100 == 0)
                SDL_RenderPresent(renderer);
        }
    }

    SDL_RenderPresent(renderer);
}