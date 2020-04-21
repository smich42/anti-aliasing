#ifndef ANTI_ALIASING_ANTIALIASING_H
#define ANTI_ALIASING_ANTIALIASING_H

#include <stdint.h>
#include <SDL2/SDL_pixels.h>

#include "demo.h"

void super_sample(Canvas *dense, Canvas *canvas, uint16_t side, uint16_t density);

void low_pass_filter(Canvas *src, Canvas *canvas, uint16_t filter_side);

float **get_filter(uint16_t filter_side);

#endif //ANTI_ALIASING_ANTIALIASING_H
