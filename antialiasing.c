#include "antialiasing.h"

void super_sample(SDL_Colour **dense, uint16_t w, uint16_t h, uint8_t div)
{
    printf("Applying Super-Sampling filter...\n");
    // Take top-left pixel of every square
    for (int x = 0; x < w - div; x += div)
        for (int y = 0; y < h - div; y += div)
        {
            // Take every pixel of the square with top-left pixel at (x,y)
            uint16_t r, g, b, a;
            r = g = b = a = 0;

            for (int i = 0; i < div; ++i)
                for (int j = 0; j < div; ++j)
                {
                    r += dense[x + i][y + j].r;
                    g += dense[x + i][y + j].g;
                    b += dense[x + i][y + j].b;
                    a += dense[x + i][y + j].a;
                }

            SDL_Colour new_colour = {r / (div * div),
                                     g / (div * div),
                                     b / (div * div),
                                     a / (div * div)};

            for (int i = 0; i < div; ++i)
                for (int j = 0; j < div; ++j)
                    dense[x + i][y + j] = new_colour;
        }
}

void low_pass(SDL_Colour **canvas, uint16_t w, uint16_t h)
{}