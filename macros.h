#ifndef ANTI_ALIASING_MACROS_H
#define ANTI_ALIASING_MACROS_H

#define MAX(a, b)             \
({                            \
    __typeof__ (a) _a = (a);  \
    __typeof__ (b) _b = (b);  \
    _a > _b ? _a : _b;        \
})

#endif //ANTI_ALIASING_MACROS_H
