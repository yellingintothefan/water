#include "Scanline.h"

#include "util.h"

// Modulous modify a pixel. Discards alpha. Great for lighting.
static uint32_t mod(const uint32_t pixel, const int modding)
{
    const uint32_t r = (((pixel >> 0x10) /****/) * modding) >> 0x08; // Shift right by 0x08 is same as
    const uint32_t g = (((pixel >> 0x08) & 0xFF) * modding) >> 0x08; // dividing by 256. Somehow
    const uint32_t b = (((pixel /*****/) & 0xFF) * modding) >> 0x08; // ofast was not catching this.
    return r << 0x10 | g << 0x08 | b;
}

// Pixel getter.
static uint32_t pget(const SDL_Surface* const surface, const Point offset)
{
    const int row = surface->h * xdec(offset.y);
    const int col = surface->w * xdec(offset.x);
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    return pixels[col + row * surface->w];
}

// Pixel putter.
static void pput(const Scanline sl, const int x, const uint32_t pixel)
{
    sl.display.pixels[x + sl.y * sl.display.width] = pixel;
}

static void xfer(const Scanline sl, const int x, const Point offset, const int tile, const int distance)
{
    pput(sl, x, mod(pget(sl.sdl.surfaces.surface[tile], offset), distance));
}

// Wall rasterer.
static void wraster(const Scanline sl, const Ray r)
{
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        // Get pixel.
        const Point offset = { (x - r.proj.bot) / r.proj.size, r.hit.offset };
        // Shade and transfer pixel.
        xfer(sl, x, offset, r.hit.surface, xilluminate(r.torch, r.corrected.x));
    }
}

// Floor rasterer.
static void fraster(const Scanline sl, const Ray r, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        // Get pixel.
        const Point offset = xlerp(r.trace, xfcast(r.proj, x));
        const int tile = xtile(offset, map.floring);
        // Do not render where there will be a pit.
        if(!tile) continue;
        // Shade and transfer pixel.
        xfer(sl, x, offset, tile, xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Ceiling rasterer.
static void craster(const Scanline sl, const Ray r, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Get pixel.
        const Point offset = xlerp(r.trace, xccast(r.proj, x));
        const int tile = xtile(offset, map.ceiling);
        // Do not render where there will a second ceiling.
        if(!tile) continue;
        // Shade and transfer pixel.
        xfer(sl, x, offset, tile, xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Second ceiling rasterer.
static void sraster(const Scanline sl, const Ray r, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Get pixel.
        const Point offset = xlerp(r.trace, xccast(r.proj, x));
        // Do not render where there was ceiling.
        if(xtile(offset, map.ceiling)) continue;
        // Shade and transfer pixel.
        xfer(sl, x,
            offset,
            '#' - ' ',
            xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Pit rasterer.
static void praster(const Scanline sl, const Ray r, const Map map, const Current current)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        // Get pixel.
        const Point offset = xlerp(r.trace, xfcast(r.proj, x));
        // Do not render where there was a floor.
        if(xtile(offset, map.floring)) continue;
        // Shade and transfer pixel.
        xfer(sl, x,
            xabs(xsub(offset, current.where)),
            '%' - ' ',
            xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Upper level rasterer (second ceiling and upper walls).
static void uraster(const Scanline sl, const Hits hits, const Hero hero, const Map map)
{
    int link = 0;
    for(Hit* hit = hits.ceiling, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;
        const Ray hind = xcalc(hero, *which, 2.0, sl.sdl.yres);
        if(link++ == 0) sraster(sl, hind, map);
        wraster(sl, hind);
    }
}

// Lower level rasterer (pit and lower walls).
static void lraster(const Scanline sl, const Hits hits, const Hero hero, const Map map, const Current current)
{
    int link = 0;
    for(Hit* hit = hits.floring, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;
        const Ray hind = xcalc(hero, *which, current.height, sl.sdl.yres);
        if(link++ == 0) praster(sl, hind, map, current);
        wraster(sl, hind);
    }
}

// Eye level rasterer. Returns a z-buffer element for the sprites.
static Point eraster(const Scanline sl, const Hits hits, const Hero hero, const Map map)
{
    const Ray ray = xcalc(hero, hits.walling, 0.0, sl.sdl.yres);
    wraster(sl, ray);
    fraster(sl, ray, map);
    craster(sl, ray, map);
    return ray.corrected;
}

// Debugging highlighter for finding uncolored pixels.
static inline void fill(const Scanline sl, const uint32_t color)
{
    for(int x = 0; x < sl.sdl.yres; x++) pput(sl, x, color);
}

Point xraster(const Scanline sl, const Hits hits, const Hero hero, const Current current, const Map map)
{
    uraster(sl, hits, hero, map);
    lraster(sl, hits, hero, map, current);
    return
    eraster(sl, hits, hero, map);
}
