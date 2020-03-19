#include "draw.h"

void read_and_draw_line(Demo *running_demo, SDL_Colour **canvas, FILE *in)
{
    coords initial, final;

    fscanf(in, "%hu %hu", &initial.x, &final.x);
    fscanf(in, "%hu %hu", &initial.y, &final.y);

    fprintf(in, "-----------------\n");

    uint16_t steps;
    draw_coords(running_demo, bresenham(initial, final, &steps), canvas, &steps);
    // draw_coords(running_demo, get_dense_line(initial, final, canvas, 4, &steps), canvas, &steps);
}

void draw_coords(Demo *running_demo, coords *to_draw, SDL_Colour **canvas, const uint16_t *steps)
{
    for (int i = 0; i < *steps; ++i)
    {
        SDL_SetRenderDrawColor(running_demo->display.renderer, to_draw[i].colour.r, to_draw[i].colour.g,
                               to_draw[i].colour.b,
                               to_draw[i].colour.a);

        SDL_RenderDrawPoint(running_demo->display.renderer, to_draw[i].x, to_draw[i].y);
        canvas[to_draw[i].x][to_draw[i].y] = to_draw[i].colour;

        SDL_RenderPresent(running_demo->display.renderer);
    }

    free(to_draw);
}

coords *get_dense_line(coords initial, coords final, SDL_Colour **canvas, uint8_t side, uint16_t *steps)
{
    uint16_t dx = abs(final.x - initial.x);
    uint16_t dy = abs(final.y - initial.y);

    *steps = MAX(dx, dy) * side * side;

    coords *line = malloc(sizeof(coords) * (*steps));

    uint16_t gradient = 2 * dy;
    int error = gradient - dx;

    uint16_t x = initial.x;
    uint16_t y = initial.y;

    for (int i = 0; x <= final.x - side; i += side * side)
    {
        int cnt = 0;
        for (int j = 0; j < side; ++j)
            for (int k = 0; k < side; ++k)
            {
                line[i + cnt].x = x + j;
                line[i + cnt].y = y + k;

                line[i + cnt].colour.r = 255;
                line[i + cnt].colour.g = 0;
                line[i + cnt].colour.b = 0;
                line[i + cnt].colour.a = 255;

                canvas[x + j][y + k] = line[i + cnt].colour;

                ++cnt;
            }

        if ((error += gradient) >= 0)
        {
            error -= 2 * dx;
            ++y;
        }
        ++x;
    }

    return line;
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
