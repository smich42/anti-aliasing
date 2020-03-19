#ifndef ANTI_ALIASING_DEMO_H
#define ANTI_ALIASING_DEMO_H

#include <stdio.h>
#include "SDL2/SDL.h"

typedef struct
{
    struct
    {
        SDL_Window *window;
        SDL_Renderer *renderer;

        uint16_t width, height;
        char *name;
    } display;

    SDL_bool is_running;
} Demo;

void demo_init(Demo *to_init, uint16_t w, uint16_t h, char *title);
void demo_quit(Demo *to_quit);

#endif //ANTI_ALIASING_DEMO_H
