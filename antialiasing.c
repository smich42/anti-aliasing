#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "antialiasing.h"

float filters[6][12][12] = {
        {
                {1.0f}
        },
        {
                {0.025f, 0.1f,   0.025f},
                {0.1f,   0.5f,   0.1f},
                {0.025f, 0.1f,   0.025f}
        },
        {
                {0.024f, 0.024f, 0.024f, 0.024f, 0.024f},
                {0.024f, 0.024f, 0.1f,   0.024f, 0.024f},
                {0.024f, 0.1f,   0.5f,   0.1f,   0.024f},
                {0.024f, 0.024f, 0.1f,   0.024f, 0.024f},
                {0.024f, 0.024f, 0.024f, 0.024f, 0.024f}
        },
        {
                {0.01f,  0.01f,  0.01f,  0.01f,  0.01f,  0.01f,  0.01f},
                {0.01f,  0.01f,  0.01f,  0.01f,  0.01f,  0.01f,  0.01f},
                {0.01f,  0.01f,  0.01f,  0.015f, 0.01f,  0.01f,  0.01f},
                {0.01f,  0.01f,  0.015f, 0.5f,   0.015f, 0.01f,  0.01f},
                {0.01f,  0.01f,  0.015f, 0.015f, 0.015f, 0.01f, 0.01f},
                {0.01f,  0.01f,  0.01f,  0.01f,  0.01f, 0.01f,  0.01f},
                {0.01f,  0.01f,  0.01f,  0.01f,  0.01f,  0.01f,  0.01f}
        },
        {
                {0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f},
                {0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f},
                {0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f},
                {0.006f, 0.006f, 0.006f, 0.008f, 0.01f,  0.008f, 0.006f, 0.006f, 0.006f},
                {0.006f, 0.006f, 0.006f, 0.01f,  0.5f,   0.01f, 0.006f, 0.006f, 0.006f},
                {0.006f, 0.006f, 0.006f, 0.008f, 0.01f, 0.008f, 0.006f, 0.006f, 0.006f},
                {0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f},
                {0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f},
                {0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f, 0.006f}
        },
        {
                {0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f},
                {0.001f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.001f},
                {0.001f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.001f},
                {0.001f, 0.004f, 0.004f, 0.004f, 0.02f,  0.02f,  0.02f,  0.004f, 0.004f, 0.004f, 0.001f},
                {0.001f, 0.004f, 0.004f, 0.02f,  0.01f,  0.1f,  0.01f,  0.02f,  0.004f, 0.004f, 0.001f},
                {0.001f, 0.004f, 0.004f, 0.02f,  0.1f,  0.5f,   0.1f,   0.02f,  0.004f, 0.004f, 0.001f},
                {0.001f, 0.004f, 0.004f, 0.02f,  0.01f,  0.1f,   0.01f,  0.02f,  0.004f, 0.004f, 0.001f},
                {0.001f, 0.004f, 0.004f, 0.004f, 0.02f,  0.02f,  0.02f,  0.004f, 0.004f, 0.004f, 0.001f},
                {0.001f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.001f},
                {0.001f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.004f, 0.001f},
                {0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f}
        }
};

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

void low_pass_filter(Canvas *src, Canvas *canvas, uint16_t filter_side)
{
    if (filter_side % 2 == 0)
        return;

    printf("Applying Low-Pass Filter with side = %d\n", filter_side);

    float **filter = get_filter(filter_side);

    for (int x = 0; x < src->w - filter_side; ++x)
        for (int y = 0; y < src->h - filter_side; ++y)
        {
            float new_r = 0;
            float new_g = 0;
            float new_b = 0;
            float new_a = 0;

            for (int i = 0; i < filter_side; ++i)
                for (int j = 0; j < filter_side; ++j)
                {
                    new_r += ((float) src->data[x + i][y + j].r) * filter[i][j];
                    new_g += ((float) src->data[x + i][y + j].g) * filter[i][j];
                    new_b += ((float) src->data[x + i][y + j].b) * filter[i][j];
                    new_a += ((float) src->data[x + i][y + j].a) * filter[i][j];
                }

            uint16_t mid_x = x + filter_side / 2 + 1;
            uint16_t mid_y = y + filter_side / 2 + 1;

            canvas->data[mid_x][mid_y].r = new_r;
            canvas->data[mid_x][mid_y].g = new_g;
            canvas->data[mid_x][mid_y].b = new_b;
            canvas->data[mid_x][mid_y].a = new_a;
        }
}

float **get_filter(uint16_t filter_side)
{
    uint8_t filter_index = (filter_side - 1) / 2;

    float **return_filter = malloc(sizeof(float *) * filter_side);

    for (int i = 0; i < filter_side; ++i)
    {
        return_filter[i] = malloc(sizeof(float) * filter_side);

        for (int j = 0; j < filter_side; ++j)
        {
            return_filter[i][j] = filters[filter_index][i][j];
        }
    }

    return return_filter;
}