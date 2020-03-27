#include <stdio.h>

#include "antialiasing.h"

void super_sample(SDL_Colour **dense, SDL_Colour **canvas, uint16_t w, uint16_t h, uint16_t side, uint16_t density)
{
    printf("Applying Super-Sampling filter...\n");

    uint16_t step = side / density;

    // Take top-left point of every pixel
    for (int x = 0; x < w - side; x += side)
        for (int y = 0; y < h - side; y += side)
        {
            uint16_t r, g, b, a;
            r = g = b = a = 0;

            // Take every point of the pixel
            for (int i = 0; i < side; i += step)
                for (int j = 0; j < side; j += step)
                {
                    r += dense[x + i][y + j].r;
                    g += dense[x + i][y + j].g;
                    b += dense[x + i][y + j].b;
                    a += dense[x + i][y + j].a;
                }

            SDL_Colour new_colour = {r / (density * density),
                                     g / (density * density),
                                     b / (density * density),
                                     a / (density * density)};

            canvas[x / side][y / side] = new_colour;
        }
}

void low_pass(SDL_Colour **canvas, uint16_t w, uint16_t h)
{}