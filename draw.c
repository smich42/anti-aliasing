#include "draw.h"
#include "macros.h"

void save_coords(SDL_Colour **canvas, coords *to_save, const uint16_t *steps)
{
    for (int i = 0; i < *steps; ++i)
        canvas[to_save[i].x][to_save[i].y] = to_save[i].colour;
}

void show_canvas(SDL_Renderer *renderer, SDL_Colour **canvas, uint16_t w, uint16_t h, bool animate)
{
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j)
        {
            SDL_SetRenderDrawColor(renderer,
                                   canvas[i][j].r,
                                   canvas[i][j].g,
                                   canvas[i][j].b,
                                   canvas[i][j].a);

            SDL_RenderDrawPoint(renderer, i, j);
            if (animate)
                SDL_RenderPresent(renderer);
        }

    if (!animate)
        SDL_RenderPresent(renderer);
}

void show_coords(SDL_Renderer *renderer, coords *to_draw, bool animate, const uint16_t *steps)
{
    for (int i = 0; i < *steps; ++i)
    {
        SDL_SetRenderDrawColor(renderer,
                               to_draw[i].colour.r,
                               to_draw[i].colour.g,
                               to_draw[i].colour.b,
                               to_draw[i].colour.a);

        SDL_RenderDrawPoint(renderer, to_draw[i].x, to_draw[i].y);

        if (animate)
            SDL_RenderPresent(renderer);
    }
    if (!animate)
        SDL_RenderPresent(renderer);
}

coords *bresenham(coords initial, coords final, uint8_t side, bool dense, uint16_t *steps)
{
    uint16_t dx = abs(final.x - initial.x);
    uint16_t dy = abs(final.y - initial.y);

    *steps = MAX(dx, dy) * side * side;

    coords *line = malloc(sizeof(coords) * (*steps));

    uint16_t x = initial.x;
    uint16_t y = initial.y;

    float slope = (float) dy / (float) dx;

    if (slope < 1)
    {
        int d = 2 * dy - dx;
        int dT = 2 * (dy - dx);
        uint16_t dS = 2 * dy;

        for (int i = 0; x < final.x - side; i += side * side)
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

                    ++cnt;
                }

            if (d < 0)
                d += dS;
            else
            {
                d += dT;
                y += dense ? 1 : side;
            }

            x += dense ? 1 : side;
        }
    }
    else
    {
        int d = 2 * dx - dy;
        int dT = 2 * (dx - dy);
        uint16_t dS = 2 * dx;

        for (int i = 0; y < final.y - side; i += side * side)
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

                    ++cnt;
                }

            if (d < 0)
                d += dS;
            else
            {
                d += dT;
                x += dense ? 1 : side;
            }

            y += dense ? 1 : side;
        }
    }

    return line;
}