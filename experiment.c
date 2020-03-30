#include <stdio.h>

#include "experiment.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

double colour_similarity(SDL_Colour a, SDL_Colour b)
{
    double total_similarity = 0;


    if (a.r != 0 || b.r != 0)
        total_similarity += MIN((double) a.r, (double) (b.r)) / MAX((double) a.r, (double) b.r);
    else
        total_similarity += 1 - (MAX((double) a.r, (double) b.r) / 255);

    if (a.g != 0 || b.g != 0)
        total_similarity += MIN((double) a.g, (double) (b.g)) / MAX((double) a.g, (double) b.g);
    else
        total_similarity += 1 - (MAX((double) a.g, (double) b.g) / 255);

    if (a.b != 0 || b.b != 0)
        total_similarity += MIN((double) a.b, (double) (b.b)) / MAX((double) a.b, (double) b.b);
    else
        total_similarity += 1 - (MAX((double) a.b, (double) b.b) / 255);

    return total_similarity / 3.0f;
}

double compare_canvasses(Canvas *A, Canvas *B)
{
    if ((A->w != B->w) || (A->h != B->h))
        return -1;

    double similarity = 0;
    uint32_t pixel_num = A->w * A->h;

    for (int i = 0; i < A->w; ++i)
        for (int j = 0; j < A->h; ++j)
            if (A->data[i][j].r == 0 && B->data[i][j].r == 0 &&
                A->data[i][j].g == 0 && B->data[i][j].g == 0 &&
                A->data[i][j].b == 0 && B->data[i][j].b == 0)
                --pixel_num;
            else
                similarity += colour_similarity(A->data[i][j], B->data[i][j]) / (double) pixel_num;

    return similarity;
}