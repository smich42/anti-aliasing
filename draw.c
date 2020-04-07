#include <stdlib.h>
#include <stdint.h>

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

        for (int i = 0; x < canvas->w - side && y < canvas->h - side; ++i)
        {
            for (int add_x = 0; add_x < side; ++add_x)
                for (int add_y = 0; add_y < side; ++add_y)
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

        for (int i = 0; x < canvas->w - side && y < canvas->h - side; ++i)
        {
            for (int add_x = 0; add_x < side; ++add_x)
                for (int add_y = 0; add_y < side; ++add_y)
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

