#include <stdio.h>
#include <stdlib.h>

#include "demo.h"

void demo_init(Demo *to_init, uint16_t w, uint16_t h, char *title)
{
    printf("Initialising...\n");

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_CreateWindowAndRenderer(w, h, 0, &to_init->display.window, &to_init->display.renderer);
    SDL_SetWindowTitle(to_init->display.window, title);

    to_init->is_running = SDL_TRUE;
}

void demo_quit(Demo *to_quit)
{
    printf("Quitting...\n");

    SDL_DestroyWindow(to_quit->display.window);
    SDL_DestroyRenderer(to_quit->display.renderer);

    SDL_Quit();
}

void canvas_init(Canvas *canvas, uint16_t w, uint16_t h)
{
    canvas->w = w;
    canvas->h = h;

    canvas->data = malloc(canvas->w * sizeof(SDL_Colour *));

    for (int i = 0; i < canvas->w; ++i)
    {
        canvas->data[i] = malloc(canvas->h * sizeof(SDL_Colour));

        for (int j = 0; j < canvas->h; ++j)
        {
            canvas->data[i][j].r = 0;
            canvas->data[i][j].g = 0;
            canvas->data[i][j].b = 0;
            canvas->data[i][j].a = 0;
        }
    }
}

void canvas_reset(Canvas *canvas)
{
    for (int i = 0; i < canvas->w; ++i)
        for (int j = 0; j < canvas->h; ++j)
        {
            canvas->data[i][j].r = 0;
            canvas->data[i][j].g = 0;
            canvas->data[i][j].b = 0;
        }
}

Canvas *canvas_enlarge(Canvas *canvas, uint16_t scale)
{
    Canvas *new_canvas = malloc(sizeof(Canvas));

    canvas_init(new_canvas, scale * canvas->w, scale * canvas->h);

    for (int i = 0; i < canvas->w; ++i)
        for (int j = 0; j < canvas->h; ++j)
        {
            for (int x = 0; x < scale; ++x)
                for (int y = 0; y < scale; ++y)
                    new_canvas->data[i * scale + x][j * scale + y] = canvas->data[i][j];
        }

    return new_canvas;
}

void canvas_show(SDL_Renderer *renderer, Canvas *canvas, bool animate)
{
    for (int i = 0; i < canvas->w; ++i)
    {
        for (int j = 0; j < canvas->h; ++j)
        {
            SDL_SetRenderDrawColor(renderer,
                                   canvas->data[i][j].r,
                                   canvas->data[i][j].g,
                                   canvas->data[i][j].b,
                                   canvas->data[i][j].a);

            SDL_RenderDrawPoint(renderer, i, j);

            // Refresh animation every 5000 pixels drawn
            if (animate && ((i + 1) * (j + 1)) % 5000 == 0)
                SDL_RenderPresent(renderer);
        }
    }

    SDL_RenderPresent(renderer);
}