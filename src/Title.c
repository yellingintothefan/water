#include "Title.h"

#include "util.h"

#include <stdarg.h>

typedef struct
{
    int start;
    int now;
    int end;
    int linger;
    char* str;
}
Title;

static Title* tt = NULL;

void xttadvance(const int now)
{
    tt->now = now;
}

void xttset(const int start, const int end, const int linger, const char* const text, ...)
{
    va_list args;
    va_start(args, text);

    free(tt->str);
    tt->str = fmts(text, args);

    // If lingering the max alpha for the alpha will be used after 50% sine in/out fade.
    tt->linger = linger;
    tt->start = start;
    tt->end = end;

    va_end(args);
}

void xttclear(void)
{
    xttset(0, 0, false, "");
}

void xttstuckclear(void)
{
    if(tt->linger)
        xttclear();
}

void xttinit(void)
{
    tt = xwipe(Title, 1);
    xttclear();
}

static int done(void)
{
    return !tt->linger && tt->now > tt->end;
}

void xttshow(const Text text, const Sdl sdl)
{
    if(done())
        return;
    const float percent = (tt->now - tt->start) / (float) (tt->end - tt->start);
    const float max = 0xFF;
    const float alpha = max * sinf(percent * FPI);
    xfputmd(
        text.fill,
        text.line,
        tt->str,
        (percent > 0.5f && tt->linger) ? max : alpha, // If lingering, do not fade out.
        sdl.renderer,
        sdl.xres / 2,
        sdl.yres / 2);
}
