#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

#include "antialiasing.h"
#include "demo.h"
#include "draw.h"
#include "experiment.h"

#define PIXEL_SIDE 12

#define DISPLAY_TITLE "Anti-aliasing Demo"

#define WINDOW_W 840
#define WINDOW_H 720

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

void run_demo(void);

int main(int argc, char **argv)
{
    demo_init(&MainDemo, WINDOW_W, WINDOW_H, DISPLAY_TITLE);

    SDL_RenderClear(MainDemo.display.renderer);
    SDL_RenderPresent(MainDemo.display.renderer);

    SDL_Event e;
    bool demo_run = false;

    while (MainDemo.is_running)
    {
        while (SDL_PollEvent(&e))
            switch (e.type)
            {
                case SDL_QUIT:
                    MainDemo.is_running = SDL_FALSE;
                    break;

                default:
                    if (!demo_run)
                    {
                        run_demo();
                        demo_run = true;
                    }
                    break;
            }
    }

    demo_quit(&MainDemo);

    return 0;
}

void run_demo(void)
{
    Canvas *canvas = malloc(sizeof(Canvas));
    Canvas *dense = malloc(sizeof(Canvas));
    Canvas *compare = malloc(sizeof(Canvas));

    canvas_init(canvas, CANVAS_W, CANVAS_H);
    canvas_init(dense, WINDOW_W, WINDOW_H);
    canvas_init(compare, WINDOW_W, WINDOW_H);

    FILE *rf = fopen("in.txt", "r");

    uint16_t N;
    fscanf(rf, "%hu", &N);

    coords input[N][2];
    SDL_Colour colours[N];

    for (int i = 0; i < N; ++i)
    {
        fscanf(rf, "%hu %hu %hu %hu", &colours[i].r, &colours[i].g, &colours[i].b, &colours[i].a);

        fscanf(rf, "%hu %hu", &input[i][0].x, &input[i][1].x);
        fscanf(rf, "%hu %hu", &input[i][0].y, &input[i][1].y);

        bresenham(input[i][0], input[i][1], PIXEL_SIDE, PIXEL_SIDE, compare, colours[i]);

        coords initial = input[i][0];
        coords final = input[i][1];

        initial.x /= PIXEL_SIDE;
        initial.y /= PIXEL_SIDE;

        final.x /= PIXEL_SIDE;
        final.y /= PIXEL_SIDE;

        bresenham(initial, final, 1, 1, canvas, colours[i]);
    }

    Canvas *ss[(int) PIXEL_SIDE / 2]; // Could instead use cbrt(PIXEL_SIDE) as an upper bound for number of divisors

    int cnt = 0;
    for (int density = 1; density <= PIXEL_SIDE; ++density)
    {
        if (PIXEL_SIDE % density == 0)
        {
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);

            for (int i = 0; i < N; ++i)
                bresenham(input[i][0], input[i][1], PIXEL_SIDE, density, dense, colours[i]);

            ss[cnt] = malloc(sizeof(Canvas));
            canvas_init(ss[cnt], CANVAS_W, CANVAS_H);

            super_sample(dense, ss[cnt], PIXEL_SIDE, density);

            clock_gettime(CLOCK_MONOTONIC, &end);

            float seconds_elapsed =
                    (float) (end.tv_sec - start.tv_sec) + (float) (end.tv_nsec - start.tv_nsec) / 1000000000.0f;

            ss[cnt] = canvas_enlarge(ss[cnt], PIXEL_SIDE);

            printf("\tSimilarity: %.2f%%\n", 100 * compare_canvasses(compare, ss[cnt]));
            printf("\tSimilarity: %.2f%%\n", 100 * compare_canvasses(compare, ss[cnt]));
            printf("\tTime elapsed: %.2lf ms\n\n", seconds_elapsed * 1000);

            if (!ANIMATE)
                sleep(1);

            canvas_show(MainDemo.display.renderer, ss[cnt], ANIMATE);

            canvas_reset(dense);

            canvas_free(ss[cnt]);
            free(ss[cnt]);

            if (!ANIMATE)
                sleep(2);

            ++cnt;
        }
    }

    Canvas *lp[(int) PIXEL_SIDE / 2];

    cnt = 0;
    for (int side = 1; side <= PIXEL_SIDE; side += 2)
    {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        lp[cnt] = malloc(sizeof(Canvas));
        canvas_init(lp[cnt], CANVAS_W, CANVAS_H);

        low_pass_filter(canvas, lp[cnt], side);

        clock_gettime(CLOCK_MONOTONIC, &end);

        float seconds_elapsed =
                (float) (end.tv_sec - start.tv_sec) + (float) (end.tv_nsec - start.tv_nsec) / 1000000000.0f;

        lp[cnt] = canvas_enlarge(lp[cnt], PIXEL_SIDE);

        printf("\tSimilarity: %.2f%%\n", 100 * compare_canvasses(compare, lp[cnt]));
        printf("\tSimilarity: %.2f%%\n", 100 * compare_canvasses(compare, lp[cnt]));
        printf("\tTime elapsed: %.2lf ms\n\n", seconds_elapsed * 1000);

        if (!ANIMATE)
            sleep(1);

        canvas_show(MainDemo.display.renderer, lp[cnt], ANIMATE);

        canvas_reset(dense);

        canvas_free(lp[cnt]);
        free(lp[cnt]);

        if (!ANIMATE)
            sleep(2);

        ++cnt;
    }

    canvas_free(canvas);

    free(dense);
    free(compare);

    printf("Finished running demo.\n");
}
