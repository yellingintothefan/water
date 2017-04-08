#include "Hero.h"
#include "Portals.h"
#include "Gpu.h"
#include "Map.h"
#include "Sprites.h"
#include "Util.h"

void play(const char* argv[])
{
    const int res = strtol(argv[1], NULL, 0);
    Map map = open("start");
    Sprites sprites = wake("start");
    Hero hero = spawn("hero.cfg");
    const Portals portals = populate("portals.cfg");
    const Gpu gpu = setup(res, "surfaces.cfg");
    #if 0
    for(int i = 0; i < 60; i++)
    #else
    while(!done())
    #endif
    {
        hero = move(hero, map.walling);
        hero = spin(hero);
        const int ch = handle(hero, map.walling);
        if(ch)
        {
            const Portal portal = portals.portal[ch - 'a'];
            map = reopen(map, portal.name);
            hero = teleport(hero, portal);
            sprites = swap(sprites, portal.name);
        }
        const Sprites updated = update(sprites, hero);
        render(hero, updated, map, res, gpu);
        free(updated.sprite);
    }
    close(map);
    sleep(sprites);
    destroy(portals);
    release(gpu);
}
