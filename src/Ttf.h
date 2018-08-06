#pragma once

#include "Font.h"

typedef struct
{
    Font fill;
    Font line;
}
Ttf;

Ttf xttfnew(const char* const path, const int size, const uint32_t inner, const uint32_t outer);
