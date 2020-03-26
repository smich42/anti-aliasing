#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "antialiasing.h"
#include "demo.h"
#include "draw.h"

#define SAMPLING_DENSITY 2
#define PIXEL_SIDE 8

#define DISPLAY_TITLE "Anti-aliasing Demo"

#define DEFAULT_W 800
#define DEFAULT_H 600

#define CANVAS_W (DEFAULT_W / PIXEL_SIDE)
#define CANVAS_H (DEFAULT_H / PIXEL_SIDE)

#define ANIMATE_LINE_DRAWING true
#define ANIMATE_ANTI_ALIASING true

Demo MainDemo = {
        {
                NULL,
                NULL,
                DEFAULT_W,
                DEFAULT_H,
                DISPLAY_TITLE,
        },
        SDL_FALSE
};

int main(int argc, char **argv) {
    demo_init(&MainDemo, DEFAULT_W, DEFAULT_H, DISPLAY_TITLE);

    SDL_RenderClear(MainDemo.display.renderer);
    SDL_RenderPresent(MainDemo.display.renderer);

    SDL_Colour **canvas = malloc(CANVAS_W * sizeof(SDL_Colour *));
    for (int i = 0; i < CANVAS_W; ++i) {
        canvas[i] = malloc(DEFAULT_H * sizeof(SDL_Colour));

        for (int j = 0; j < DEFAULT_H / SAMPLING_DENSITY; ++j) {
            canvas[i][j].r = 0;
            canvas[i][j].g = 0;
            canvas[i][j].b = 0;
            canvas[i][j].a = 0;
        }
    }

    SDL_Colour **dense = malloc(DEFAULT_W * sizeof(SDL_Colour *));
    for (int i = 0; i < DEFAULT_W; ++i) {
        dense[i] = malloc(DEFAULT_H * sizeof(SDL_Colour));

        for (int j = 0; j < DEFAULT_H; ++j) {
            dense[i][j].r = 0;
            dense[i][j].g = 0;
            dense[i][j].b = 0;
            dense[i][j].a = 0;
        }
    }

    FILE *rf = fopen("in.txt", "r");

    uint16_t N;
    fscanf(rf, "%hu", &N);

    coords initial, final;
    uint16_t steps;

    uint16_t r, g, b, a;
    SDL_Colour line_colour;

    for (int i = 0; i < N; ++i) {
        fscanf(rf, "%hu %hu %hu %hu", &r, &g, &b, &a);
        fscanf(rf, "%hu %hu", &initial.x, &final.x);
        fscanf(rf, "%hu %hu", &initial.y, &final.y);

        line_colour.r = r;
        line_colour.g = g;
        line_colour.b = b;
        line_colour.a = a;

        coords *new_line_dense = bresenham(initial, final, line_colour);
        save_coords(dense, new_line_dense, DEFAULT_W, DEFAULT_H, PIXEL_SIDE, SAMPLING_DENSITY);

        initial.x /= PIXEL_SIDE;
        initial.y /= PIXEL_SIDE;

        final.x /= PIXEL_SIDE;
        final.y /= PIXEL_SIDE;

        coords *new_line = bresenham(initial, final, line_colour);
        save_coords(canvas, new_line, CANVAS_W, CANVAS_H, PIXEL_SIDE, 1);
        show_coords(MainDemo.display.renderer, new_line, CANVAS_W, CANVAS_H, PIXEL_SIDE, ANIMATE_LINE_DRAWING);

        free(new_line);
        free(new_line_dense);
    }

    if (!ANIMATE_ANTI_ALIASING)
        sleep(2);

    //super_sample(dense, DEFAULT_W, DEFAULT_H, PIXEL_SIDE, SAMPLING_DENSITY);
    show_canvas(MainDemo.display.renderer, dense, DEFAULT_W, DEFAULT_H, ANIMATE_ANTI_ALIASING);

    SDL_Event e;
    while (MainDemo.is_running) {
        while (SDL_PollEvent(&e))
            switch (e.type) {
                case SDL_QUIT:
                    MainDemo.is_running = SDL_FALSE;
                    break;
            }
    }

    demo_quit(&MainDemo);

    return 0;
}
