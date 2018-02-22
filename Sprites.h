#pragma once

#include "Hero.h"
#include "Attack.h"
#include "Sprite.h"
#include "Field.h"

typedef struct
{
    Sprite* sprite;
    int count;
    int max;
}
Sprites;

Sprites xsnew(const int max);

void xkill(const Sprites);

// Arranges sprites furthest to nearest relative to the hero's orientation (angle and position).
Sprites xorient(const Sprites, const Hero);

// Appends in new sprites with the sprite editor.
Sprites xlay(Sprites, const Map, const Overview);

// Sprite data updater.
Sprites xcaretake(Sprites, const Hero, const Input, const Map, const Attack, const Field, const int ticks);
