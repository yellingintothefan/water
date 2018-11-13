#pragma once

#include "Hero.h"
#include "Attack.h"
#include "Sprite.h"
#include "Field.h"
#include "Gauge.h"
#include "Inventory.h"
#include "Sorter.h"
#include "Method.h"
#include "Fire.h"

typedef struct
{
    Sprite* sprite;
    int count;
    int max;

    // How the sprites were last hurt.
    Method last;
}
Sprites;

Sprites s_spawn(const int max);

Sprites s_lay(Sprites, const Map, const Overview);

void s_pull(const Sprites, const Hero);

void s_push(const Sprites, const Hero);

void s_turn(const Sprites, const float yaw);

void s_sort(const Sprites, Sorter);

Sprites s_hurt(Sprites, const Attack, const Hero, const Input, const Inventory, const Timer);

Hero s_caretake(const Sprites, const Hero, const Map, const Field, const Gauge, const Fire, const Timer);

Sprites s_gen(const int max, const Map);

Sprites s_populate(Sprites, const Map);

Map s_count_agents(const Sprites, Map);

Sprites s_spread_fire(Sprites, const Fire, const Map, const Timer);
