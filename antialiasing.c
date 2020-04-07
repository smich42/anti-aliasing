#include <stdio.h>
#include <stdint.h>

#include "antialiasing.h"

void super_sample(Canvas *dense, Canvas *canvas, uint16_t side, uint16_t density)
{
    printf("Applying Super-Sampling with density = %hu\n", density);

    uint16_t step = side / density;

    // Take top-left point of every pixel
    for (int x = 0; x < dense->w - side; x += side)
        for (int y = 0; y < dense->h - side; y += side)
        {
            uint16_t r, g, b, a;
            r = g = b = a = 0;

            // Take every point of the pixel
            for (int i = 0; i < side; i += step)
                for (int j = 0; j < side; j += step)
                {
                    r += dense->data[x + i][y + j].r;
                    g += dense->data[x + i][y + j].g;
                    b += dense->data[x + i][y + j].b;
                    a += dense->data[x + i][y + j].a;
                }

            SDL_Colour new_colour = {r / (density * density),
                                     g / (density * density),
                                     b / (density * density),
                                     a / (density * density)};

            canvas->data[x / side][y / side] = new_colour;
        }
}