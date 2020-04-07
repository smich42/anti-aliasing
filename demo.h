#ifndef ANTI_ALIASING_DEMO_H
#define ANTI_ALIASING_DEMO_H

#include <stdint.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

typedef struct Demo
{
    struct display
    {
        SDL_Window *window;
        SDL_Renderer *renderer;

        uint16_t width, height;
        char *name;
    } display;

    SDL_bool is_running;
} Demo;

typedef struct Canvas
{
    SDL_Colour **data;

    uint16_t w;
    uint16_t h;
} Canvas;

void demo_init(Demo *to_init, uint16_t w, uint16_t h, char *title);

void demo_quit(Demo *to_quit);

void canvas_init(Canvas *canvas, uint16_t w, uint16_t h);

void canvas_reset(Canvas *canvas);

Canvas *canvas_enlarge(Canvas *canvas, uint16_t scale);

void canvas_show(SDL_Renderer *renderer, Canvas *canvas, bool animate);

#endif //ANTI_ALIASING_DEMO_H
