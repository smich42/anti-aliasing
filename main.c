#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

#define DEFAULT_W 800
#define DEFAULT_H 600

#define DISPLAY_TITLE "AntiAliasing Demo"

#define SS_DIV 4

#define MAX(a, b) a > b ? a : b
#define SWAP(x, y)      \
do                      \
{                       \
    typeof(x) temp = x; \
    x = y;              \
    y = temp;           \
} while (0)

typedef struct
{
    uint16_t x;
    uint16_t y;
    SDL_Colour colour;
} coords;

void demo_init(void);
void demo_quit(void);

coords *bresenham(coords initial, coords final, uint16_t *steps);
coords *SS(coords *canvas);

void draw_coords(SDL_Renderer *renderer, coords *to_draw, uint16_t side, const uint16_t *steps);
void read_and_draw_line(SDL_Renderer *renderer, uint16_t side, FILE *in);

struct
{
    void (*init)(void);
    void (*quit)(void);

    struct
    {
        SDL_Window *window;
        SDL_Renderer *renderer;

        uint16_t width, height;
        char *name;
    } display;

    SDL_bool is_running;
} Demo = {
        demo_init,
        demo_quit,
        {
                NULL,
                NULL,
                DEFAULT_W,
                DEFAULT_H,
                DISPLAY_TITLE,
        },
        SDL_FALSE
};

SDL_Colour canvas[DEFAULT_W][DEFAULT_H];
SDL_Colour dense[DEFAULT_W * SS_DIV][DEFAULT_H * SS_DIV];

int main(int argc, char **argv)
{
    Demo.init();

    SDL_RenderClear(Demo.display.renderer);
    SDL_RenderPresent(Demo.display.renderer);

    FILE *rf = fopen("in.txt", "r");
    read_and_draw_line(Demo.display.renderer, SS_DIV, rf);
    read_and_draw_line(Demo.display.renderer, SS_DIV, rf);

    SDL_Event e;
    while (Demo.is_running)
    {
        while (SDL_PollEvent(&e))
            switch (e.type)
            {
                case SDL_QUIT:
                    Demo.is_running = SDL_FALSE;
                    break;
            }
    }

    Demo.quit();

    return 0;
}

void read_and_draw_line(SDL_Renderer *renderer, uint16_t side, FILE *in)
{
    coords initial, final;

    fscanf(in, "%hu %hu", &initial.x, &final.x);
    fscanf(in, "%hu %hu", &initial.y, &final.y);

    fprintf(in, "-----------------\n");

    uint16_t steps;
    draw_coords(renderer, bresenham(initial, final, &steps), side, &steps);
}

void draw_coords(SDL_Renderer *renderer, coords *to_draw, uint16_t side, const uint16_t *steps)
{
    SDL_RenderSetScale(renderer, side, side);

    for (int i = 0; i < *steps; ++i)
    {
        SDL_SetRenderDrawColor(Demo.display.renderer, to_draw[i].colour.r, to_draw[i].colour.g, to_draw[i].colour.b,
                               SDL_ALPHA_OPAQUE);

        SDL_RenderDrawPoint(renderer, to_draw[i].x, to_draw[i].y);
        canvas[to_draw[i].x][to_draw[i].y] = to_draw[i].colour;
        
        SDL_RenderPresent(renderer);
    }

    free(to_draw);
}

coords *bresenham(coords initial, coords final, uint16_t *steps)
{
    uint16_t dx = abs(final.x - initial.x);
    uint16_t dy = abs(final.y - initial.y);

    *steps = MAX(dx, dy);
    coords *line = malloc(sizeof(coords) * (*steps));

    uint16_t gradient = 2 * dy;
    int error = gradient - dx;

    uint16_t x = initial.x;
    uint16_t y = initial.y;

    for (int i = 0; x <= final.x; ++i)
    {
        line[i].x = x;
        line[i].y = y;

        line[i].colour.r = 255;
        line[i].colour.g = 0;
        line[i].colour.b = 0;
        line[i].colour.a = 255;

        if ((error += gradient) >= 0)
        {
            error -= 2 * dx;
            ++y;
        }
        ++x;
    }

    return line;
}

coords *SS(coords *canvas)
{
    coords *new_canvas = malloc(sizeof(coords) * DEFAULT_W * DEFAULT_H);
    for (int i = 0; i < DEFAULT_W * DEFAULT_H; ++i)
    {
    }
}

void demo_init(void)
{
    printf("Initialising...\n");

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_CreateWindowAndRenderer(DEFAULT_W, DEFAULT_H, 0, &Demo.display.window, &Demo.display.renderer);
    SDL_SetWindowTitle(Demo.display.window, DISPLAY_TITLE);

    Demo.is_running = SDL_TRUE;
}

void demo_quit(void)
{
    printf("Quitting...\n");

    SDL_DestroyWindow(Demo.display.window);
    SDL_DestroyRenderer(Demo.display.renderer);

    SDL_Quit();
}
