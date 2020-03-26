#include <stdio.h>

#include "demo.h"

void demo_init(Demo *to_init, uint16_t w, uint16_t h, char *title) {
    printf("Initialising...\n");

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_CreateWindowAndRenderer(w, h, 0, &to_init->display.window, &to_init->display.renderer);
    SDL_SetWindowTitle(to_init->display.window, title);

    to_init->is_running = SDL_TRUE;
}

void demo_quit(Demo *to_quit) {
    printf("Quitting...\n");

    SDL_DestroyWindow(to_quit->display.window);
    SDL_DestroyRenderer(to_quit->display.renderer);

    SDL_Quit();
}
