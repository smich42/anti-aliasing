#include "antialiasing.h"
#include "demo.h"
#include "draw.h"
#include <stdlib.h>

#define DEFAULT_W 800
#define DEFAULT_H 600
#define DISPLAY_TITLE "Anti-aliasing Demo"

#define LINE_WIDTH 3
#define SS_DIV 2

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

int main(void) {
    demo_init(&MainDemo, DEFAULT_W, DEFAULT_H, DISPLAY_TITLE);

    SDL_RenderClear(MainDemo.display.renderer);
    SDL_RenderPresent(MainDemo.display.renderer);

    SDL_Colour **canvas = malloc(DEFAULT_W * sizeof(SDL_Colour *));
    SDL_Colour **dense = malloc(DEFAULT_W * sizeof(SDL_Colour *));

    for (int i = 0; i < DEFAULT_W; ++i) {
        canvas[i] = malloc(DEFAULT_H * sizeof(SDL_Colour));
        dense[i] = malloc(DEFAULT_H * sizeof(SDL_Colour));

        for (int j = 0; j < DEFAULT_H; ++j) {
            dense[i][j].r = canvas[i][j].r = 0;
            dense[i][j].g = canvas[i][j].g = 0;
            dense[i][j].b = canvas[i][j].b = 0;
            dense[i][j].a = canvas[i][j].a = 0;
        }
    }

    FILE *rf = fopen("in.txt", "r");

    uint16_t N;
    fscanf(rf, "%hu", &N);

    coords initial, final;
    uint16_t steps;

    for (int i = 0; i < N; ++i) {
        fscanf(rf, "%hu %hu", &initial.x, &final.x);
        fscanf(rf, "%hu %hu", &initial.y, &final.y);

        coords *new_line = bresenham(initial, final, LINE_WIDTH, false, &steps);
        save_coords(canvas, new_line, &steps);

        show_coords(MainDemo.display.renderer, new_line, true, &steps);

        coords *new_line_dense = bresenham(initial, final, LINE_WIDTH, true, &steps);
        save_coords(dense, new_line_dense, &steps);

        free(new_line);
        free(new_line_dense);
    }

    super_sample(dense, DEFAULT_W, DEFAULT_H, SS_DIV);
    show_canvas(MainDemo.display.renderer, dense, DEFAULT_W, DEFAULT_H, true);

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
