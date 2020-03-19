#include "demo.h"
#include "draw.h"

#define DEFAULT_W 800
#define DEFAULT_H 600
#define DISPLAY_TITLE "AntiAliasing Demo"

#define SS_DIV 4

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

int main(int argc, char **argv)
{
    demo_init(&MainDemo, DEFAULT_W, DEFAULT_H, DISPLAY_TITLE);

    SDL_RenderClear(MainDemo.display.renderer);
    SDL_RenderPresent(MainDemo.display.renderer);

    SDL_Colour **canvas = malloc(DEFAULT_W * sizeof(SDL_Colour *));
    SDL_Colour **dense = malloc(DEFAULT_W * SS_DIV * sizeof(SDL_Colour *));

    for (int i = 0; i < DEFAULT_W; ++i)
        canvas[i] = malloc(DEFAULT_H * sizeof(SDL_Colour));

    for (int i = 0; i < DEFAULT_W * SS_DIV; ++i)
        dense[i] = malloc(DEFAULT_H * sizeof(SDL_Colour));

    SDL_RenderSetScale(MainDemo.display.renderer, SS_DIV, SS_DIV);

    FILE *rf = fopen("in.txt", "r");
    read_and_draw_line(&MainDemo, dense, rf);
    read_and_draw_line(&MainDemo, dense, rf);

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
