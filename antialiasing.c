#include "antialiasing.h"

void super_sample(SDL_Colour **dense, uint16_t w, uint16_t h, uint16_t side, uint16_t sampling_density)
{
    printf("Applying Super-Sampling filter...\n");

    uint16_t sampling_pixel_side = side / sampling_density;
    uint16_t points_in_pixel = sampling_pixel_side * sampling_pixel_side;

    // Take top-left point of every pixel
    for (int x = 0; x < w - side; x += side)
        for (int y = 0; y < h - side; y += side)
        {
            uint16_t r, g, b, a;
            r = g = b = a = 0;

            // Take every point of the pixel
            for (int i = 0; i < sampling_pixel_side; i += side / sampling_pixel_side)
                for (int j = 0; j < sampling_pixel_side; j += side / sampling_pixel_side)
                {
                    r += dense[x + i][y + j].r;
                    g += dense[x + i][y + j].g;
                    b += dense[x + i][y + j].b;
                    a += dense[x + i][y + j].a;
                }

            SDL_Colour new_colour = {r / points_in_pixel,
                                     g / points_in_pixel,
                                     b / points_in_pixel,
                                     a / points_in_pixel};

            for (int i = 0; i < side; ++i)
                for (int j = 0; j < side; ++j)
                    dense[x + i][y + j] = new_colour;
        }
}

void low_pass(SDL_Colour **canvas, uint16_t w, uint16_t h)
{}