#include "Test.h"

#include "Util.h"
#include "Sprites.h"
#include "Hero.h"
#include "String.h"

static inline void _concat()
{
    const char* strings[] = { "each", "must", "concatenate", "another", "string", "and", "freed" };
    for(int i = 0; i < len(strings); i++)
    {
        char* const string = concat(strings[i], "...");
        printf("%s\n", string);
        free(string);
    }
}

static inline void _match()
{
    const char* strings[] = { "each", "of", "these", "must", "match", "themselves" };
    for(int i = 0; i < len(strings); i++)
        printf("%s\n", boolean(match(strings[i], strings[i])));
}

static inline void _trim()
{
    char strings[][80] = { " lorem ", "  ipsum  ", "morel ", " muspi", " notice the spaces " };
    for(int i = 0; i < len(strings); i++)
        printf("_%s_\n", trim(strings[i]));
}

static inline void __String()
{
    _concat();
    _match();
    _trim();
}

static inline void readme()
{
    FILE* const file = fopen("README", "r");
    const int lines = lns(file );
    for(int i = 0; i < lines; i++)
    {
        char* const line = readln(file);
        puts(line);
        free(line);
    }
    fclose(file);
}

static inline void license()
{
    puts("Water copyright (c) 2017 Gustav Louw, Cameron Bauer");
    puts("This program comes with ABSOLUTELY NO WARRANTY.");
    puts("This is free software, and you are welcome to redistribute it");
    puts("under certain conditions.\n");
}

extern void test()
{
    license();
    readme();
}
