#include "Util.h"
#include <SDL2/SDL.h>

int fl(const float x)
{
    return (int) x - (x < (int) x);
}

int cl(const float x)
{
    return (int) x + (x > (int) x);
}

float dec(const float x)
{
    return x - (int) x;
}

int lns(FILE* const file)
{
    int ch, lines = 0, pc = '\n';
    while((ch = getc(file)) != EOF)
    {
        if(ch == '\n')
            lines++;
        pc = ch;
    }
    if(pc != '\n')
        lines++;
    rewind(file);
    return lines;
}

bool done()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    return event.type == SDL_QUIT
        || event.key.keysym.sym == SDLK_F1
        || event.key.keysym.sym == SDLK_ESCAPE;
}

char* readln(FILE* const file)
{
    int ch, reads = 0, size = 64;
    char* line = (char*) malloc(size * sizeof(char));
    while((ch = getc(file)) != '\n' && ch != EOF)
    {
        line[reads++] = ch;
        if(reads + 1 == size)
            line = (char*) realloc(line, size *= 2);
    }
    line[reads] = '\0';
    return line;
}

char* strnew(const char* const str)
{
    char* copy = (char*) malloc(strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

char* strcon(const char* const a, const char* const b)
{
    char* c = (char*) malloc(strlen(a) + strlen(b) + 1);
    strcpy(c, a);
    strcat(c, b);
    return c;
}