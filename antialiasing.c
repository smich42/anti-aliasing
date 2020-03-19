#include "antialiasing.h"

void SS(SDL_Colour **canvas, uint8_t div, uint16_t w, uint16_t h)
{
    for (int i = 0; i < w * div; i += div)
        for (int j = 0; j < h * div; j += div)
        {

        }
}

void low_pass(SDL_Colour **canvas, uint16_t w, uint16_t h)
{}