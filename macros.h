#ifndef ANTI_ALIASING_MACROS_H
#define ANTI_ALIASING_MACROS_H

#define MAX(a, b)             \
({                            \
    __typeof__ (a) _a = (a);  \
    __typeof__ (b) _b = (b);  \
    _a > _b ? _a : _b;        \
})

#define SWAP(x, y)\
do                                                                    \
{                                                                     \
    char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1];  \
    memcpy(swap_temp, &y, sizeof(x));                                 \
    memcpy(&y, &x, sizeof(x));                                        \
    memcpy(&x, swap_temp, sizeof(x));                                 \
} while(0)

#endif //ANTI_ALIASING_MACROS_H
