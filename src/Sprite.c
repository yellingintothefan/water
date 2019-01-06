#include "Sprite.h"

#include "Speech.h"
#include "Frame.h"
#include "util.h"

#include <assert.h>
#include <ctype.h>

static Sprite init_speech(Sprite sprite, const Timer tm)
{
    static Speech zero;
    switch(sprite.ascii)
    {
    case 'b':
        sprite.speech = s_use_greet(tm);
        break;
    default:
        sprite.speech = zero;
        break;
    }
    return sprite;
}

// Be sure to map these sprites to their surface paths in Surfaces.c.
static const Sprite lower[] = {
    /* Flower  */ { 'a', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* Dwarf   */ { 'b', 0, 0, 8, 6, 1.0f, 0.038f, 0.0032f, 800.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,LETTER}, {1,LETTER}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* Ember   */ { 'c', 1, 1, 4, 0, 1.0f, 0.000f, 0.0000f,   1.0f, 0.50f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* Lootbag */ { 'd', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f,   1.0f, 0.00f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'e', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'f', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'g', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'h', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'i', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'j', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'k', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'l', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'm', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'n', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'o', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'p', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'q', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'r', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 's', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 't', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'u', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'v', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'w', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'x', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'y', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'z', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
};

static const Sprite upper[] = {
    /* */         { 'A', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'B', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'C', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'D', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'E', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'F', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'G', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'H', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'I', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'J', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'K', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'L', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'M', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'N', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'O', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'P', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'Q', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'R', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'S', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'T', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'U', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'V', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'W', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'X', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'Y', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
    /* */         { 'Z', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, {0,NONE  }, {0,NONE  }, IDLE, {0,0,0,0}, {{0},0,0,0}, 0, 0 },
};

static void check(const Sprite table[], const int len, const char first, const char last)
{
    for(int i = 0; i < len; i++)
        assert(table[i].ascii == first + i);
    assert(len == last - first + 1);
}

void s_test(void)
{
    check(lower, u_len(lower), 'a', 'z');
    check(upper, u_len(upper), 'A', 'Z');
}

Sprite s_register(const int ascii, const Point where, const Timer tm)
{
    Sprite sprite = islower(ascii) ? lower[ascii - 'a'] : upper[ascii - 'A'];
    sprite.where = sprite.last = where;
    return init_speech(sprite, tm);
}

int s_sprite(const int ascii)
{
    return isalpha(ascii);
}

int s_cosmetic(const int ascii)
{
    return ascii == 'a';
}

int s_firey(const int ascii)
{
    return ascii == 'c';
}

int s_inanimate(const int ascii)
{
    return ascii == 'd' || s_firey(ascii);
}

int s_useless(const Sprite* const sprite)
{
    return s_dead(sprite->state) || s_cosmetic(sprite->ascii);
}

int s_not_agent(const Sprite* const sprite)
{
    return s_useless(sprite) || s_inanimate(sprite->ascii);
}

int s_stuck(const Sprite* const sprite)
{
    return s_dead(sprite->state) || s_hurting(sprite->state) || s_stunned(sprite->state);
}

void s_place(Sprite* const sprite, const Point to)
{
    sprite->last = sprite->where;
    sprite->where = to;
}

int s_muted(const Sprite* const sprite)
{
    return sprite->speech.count <= 1; // Includes empty ("") padding.
}

int s_busy(const Sprite* const sprite, const Timer tm)
{
    return tm.ticks < sprite->busy_ticks;
}

void s_go_busy(Sprite* const sprite, const Timer tm, const int ticks, const State state)
{
    sprite->busy_ticks = tm.ticks + ticks;
    sprite->state = state;
}

int s_impulse(const Sprite* const sprite, const Timer tm)
{
    return s_attacking(sprite->state) && tm.rise;
}

int s_evil_act(const Sprite* const sprite, const Timer tm)
{
    return sprite->evil && tm.fall;
}

int s_will_rage(const Sprite* const sprite, const Timer tm)
{
    return sprite->state == IDLE && s_evil_act(sprite, tm);
}

int s_must_spread(const Sprite* const sprite, char** const floring)
{
    return s_firey(sprite->ascii) && s_alive(sprite->state) && p_char(sprite->where, floring) == '(';
}

// TODO: Different ticks for each sprite.
// TODO: Make a stunned animation.
void s_parried(Sprite* const sprite, const Point velocity, const Timer tm)
{
    const Point dir = p_unit(velocity);
    if((sprite->state == ATTACK_N && p_south(dir))
    || (sprite->state == ATTACK_S && p_north(dir))
    || (sprite->state == ATTACK_W && p_east (dir))
    || (sprite->state == ATTACK_E && p_west (dir)))
    {
        const int ticks = 1 + (rand() % sprite->stun_ticks);
        const int frames = ticks * FRAMES;
        s_go_busy(sprite, tm, frames, STUNNED);
    }
}
