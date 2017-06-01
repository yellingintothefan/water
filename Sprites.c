#include "Sprites.h"

#include "Point.h"
#include "Util.h"
#include "Hero.h"
#include "String.h"

static Sprite generic(const Point where)
{
    Sprite sprite;
    sprite.where = where;
    sprite.ascii = 'o';
    sprite.state = IDLE;
    sprite.transparent = false;
    sprite.width = 0.5;
    return sprite;
}

static Sprite _o(const Point where)
{
    Sprite sprite = generic(where);
    sprite.width = 0.66;
    sprite.ascii = 'o';
    return sprite;
}

static Sprite _g(const Point where)
{
    Sprite sprite = generic(where);
    sprite.width = 0.60;
    sprite.ascii = 'g';
    sprite.transparent = true;
    return sprite;
}

static Sprite _t(const Point where)
{
    Sprite sprite = generic(where);
    sprite.width = 0.33;
    sprite.ascii = 't';
    return sprite;
}

static Sprite _z(const Point where)
{
    Sprite sprite = generic(where);
    sprite.width = 0.50;
    sprite.ascii = 'z';
    return sprite;
}

static Sprite registrar(const int ascii, const Point where)
{
    switch(ascii)
    {
        case 'o': return _o(where);
        case 'g': return _g(where);
        case 't': return _t(where);
        case 'z': return _z(where);
    }
    return generic(where);
}

extern Sprites wake(const int level)
{
    char which[MINTS];
    sprintf(which, "%d", level);
    char* const path = concat("sprites/lvl", which);
    FILE* const file = fopen(path, "r");
    Sprites sprites;
    sprites.count = lns(file);
    sprites.sprite = toss(Sprite, sprites.count);
    sprites.max = sprites.count;
    for(int i = 0; i < sprites.count; i++)
    {
        char* const line = readln(file);
        char ascii = 0;
        Point where = { 0.0, 0.0 };
        sscanf(line, "%c,%f,%f\n", &ascii, &where.x, &where.y);
        sprites.sprite[i] = registrar(ascii, where);
        free(line);
    }
    fclose(file);
    free(path);
    return sprites;
}

extern void entomb(const Sprites sprites, const int level)
{
    char which[MINTS];
    sprintf(which, "%d", level);
    char* const path = concat("sprites/lvl", which);
    remove(path);
    FILE* const file = fopen(path, "w");
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        fprintf(file, "%c,%f,%f\n", sprite->ascii, sprite->where.x, sprite->where.y);
    }
    fclose(file);
    free(path);
}

extern void kill(const Sprites sprites)
{
    free(sprites.sprite);
}

static void pull(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = sub(sprite->where, hero.where);
    }
}

static void push(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = add(sprite->where, hero.where);
    }
}

static int backwards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    if(mag(pa) < mag(pb)) return +1; else if(mag(pa) > mag(pb)) return -1;
    return 0;
}

static int forewards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    if(mag(pa) < mag(pb)) return -1; else if(mag(pa) > mag(pb)) return +1;
    return 0;
}

static void sort(const Sprites sprites, const bool foreward)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite), foreward ? forewards : backwards);
}

static Sprites copy(const Sprites sprites)
{
    Sprites temps;
    temps.count = sprites.count;
    temps.max = sprites.max;
    temps.sprite = toss(Sprite, sprites.count);
    memcpy(temps.sprite, sprites.sprite, sprites.count * sizeof(Sprite));
    return temps;
}

static void turn(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = trn(sprite->where, -hero.theta);
    }
}

extern Sprites arrange(const Sprites sprites, const Hero hero)
{
    const Sprites copied = copy(sprites);
    pull(copied, hero);
    turn(copied, hero);
    sort(copied, false);
    return copied;
}

static void rearrange(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    sort(sprites, true);
    push(sprites, hero);
}

extern void rest(const Sprites sprites, const State state)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(sprite->state == state)
            sprite->state = IDLE;
    }
}

extern bool issprite(const int ascii)
{
    return isalpha(ascii);
}

static Sprites place(const Sprites sprites, const Hero hero, const Input input)
{
    if(hero.inserting)
        return sprites;
    if(!hero.consoling)
        return sprites;
    if(!(input.key[SDL_SCANCODE_K] || input.r))
        return sprites;
    if(!issprite(hero.surface))
        return sprites;
    Sprites temp = sprites;
    if(temp.count == 0)
        retoss(temp.sprite, Sprite, temp.max = 1);
    if(temp.count >= temp.max)
        retoss(temp.sprite, Sprite, temp.max *= 2);
    const Point hand = touch(hero, hero.arm);
        temp.sprite[temp.count++] = registrar(hero.surface, hand);
    return temp;
}

static void grab(const Sprites sprites, const Hero hero, const Input input)
{
    rest(sprites, GRABBED);
    if(hero.inserting)
        return;
    if(!hero.consoling)
        return;
    if(!(input.key[SDL_SCANCODE_J] || input.l))
        return;
    // Grabs one sprite
    const Point hand = touch(hero, hero.arm);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(eql(hand, sprite->where, sprite->width))
        {
            sprite->state = GRABBED;
            sprite->where = hand;
            return;
        }
    }
}

extern Sprites caretake(const Sprites sprites, const Hero hero, const Input input)
{
    rearrange(sprites, hero);
    grab(sprites, hero, input);
    return place(sprites, hero, input);
}
