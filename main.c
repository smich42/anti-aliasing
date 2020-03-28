#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "antialiasing.h"
#include "demo.h"
#include "draw.h"

#define SAMPLING_DENSITY 1
#define PIXEL_SIDE 12

#define DISPLAY_TITLE "Anti-aliasing Demo"

#define WINDOW_W 1200
#define WINDOW_H 900

#define CANVAS_W (WINDOW_W / PIXEL_SIDE)
#define CANVAS_H (WINDOW_H / PIXEL_SIDE)

#define ANIMATE true

Demo MainDemo = {
        {
                NULL,
                NULL,
                WINDOW_W,
                WINDOW_H,
                DISPLAY_TITLE,
        },
        SDL_FALSE
};

int main(int argc, char **argv)
{
    demo_init(&MainDemo, WINDOW_W, WINDOW_H, DISPLAY_TITLE);

    SDL_RenderClear(MainDemo.display.renderer);
    SDL_RenderPresent(MainDemo.display.renderer);

    Canvas *canvas = malloc(sizeof(Canvas));

    canvas->w = CANVAS_W;
    canvas->h = CANVAS_H;

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

    Canvas *dense = malloc(sizeof(Canvas));

    dense->w = WINDOW_W;
    dense->h = WINDOW_H;

    dense->data = malloc(dense->w * sizeof(SDL_Colour *));

    for (int i = 0; i < dense->w; ++i)
    {
        dense->data[i] = malloc(dense->h * sizeof(SDL_Colour));

        for (int j = 0; j < dense->h; ++j)
        {
            dense->data[i][j].r = 0;
            dense->data[i][j].g = 0;
            dense->data[i][j].b = 0;
            dense->data[i][j].a = 0;
        }
    }

    FILE *rf = fopen("in.txt", "r");

    uint16_t N;
    fscanf(rf, "%hu", &N);

    coords initial, final;

    uint16_t r, g, b, a;
    SDL_Colour line_colour;

    for (int i = 0; i < N; ++i)
    {
        fscanf(rf, "%hu %hu %hu %hu", &r, &g, &b, &a);
        fscanf(rf, "%hu %hu", &initial.x, &final.x);
        fscanf(rf, "%hu %hu", &initial.y, &final.y);

        line_colour.r = r;
        line_colour.g = g;
        line_colour.b = b;
        line_colour.a = a;

        bresenham(initial, final, PIXEL_SIDE, SAMPLING_DENSITY, dense, line_colour);

        initial.x /= PIXEL_SIDE;
        initial.y /= PIXEL_SIDE;

        final.x /= PIXEL_SIDE;
        final.y /= PIXEL_SIDE;

        bresenham(initial, final, 1, 1, canvas, line_colour);
    }

    show_canvas(MainDemo.display.renderer, canvas, PIXEL_SIDE, ANIMATE);

    // Give time before anti-aliasing happens
    if (!ANIMATE)
        sleep(2);

    // show_canvas(MainDemo.display.renderer, dense, WINDOW_W, WINDOW_H, 1, ANIMATE);

    super_sample(dense, canvas, PIXEL_SIDE, SAMPLING_DENSITY);
    show_canvas(MainDemo.display.renderer, canvas, PIXEL_SIDE, ANIMATE);

    SDL_Event e;
    while (MainDemo.is_running)
    {
        while (SDL_PollEvent(&e))
            switch (e.type)
            {
                case SDL_QUIT:
                    MainDemo.is_running = SDL_FALSE;
                    break;
            }
    }

    demo_quit(&MainDemo);

    return 0;
}
