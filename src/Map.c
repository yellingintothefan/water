#include "Map.h"

#include "Sprites.h"
#include "Title.h"
#include "util.h"

#include <math.h>

static char** reset(char** block, const int rows, const int cols, const int blok)
{
    for(int row = 0; row < rows; row++)
    for(int col = 0; col < cols; col++)
        block[row][col] = blok;
    return block;
}

static char** mnew(const int rows, const int cols, const int blok)
{
    char** block = u_toss(char*, rows);

    for(int row = 0; row < rows; row++)
        block[row] = u_toss(char, cols);

    return reset(block, rows, cols, blok);
}

Map m_gen(const int rows, const int cols, const Points trapdoors, const Points interests, const int grid)
{
    const Sheer mid = {
        0.0f,
        mid.a + (float) (rand() % 2)
    };
    const Sheer top = {
        mid.b + 1.0f,
        top.a + (float) (rand() % 2)
    };

    static Map zero;
    Map map = zero;

    map.rows = rows;
    map.cols = cols;

    map.ceiling = mnew(map.rows, map.cols, '#');
    map.walling = mnew(map.rows, map.cols, '#');
    map.floring = mnew(map.rows, map.cols, '"');

    map.trapdoors = trapdoors;
    map.rooms = r_init(interests);

    map.top = top;
    map.mid = mid;
    map.grid = grid;

    return map;
}

Theme lutheme(const Map map, const Point where)
{
    for(int i = 0; i < map.rooms.count; i++)
        if(p_eql(where, map.rooms.wheres[i], map.grid))
            return map.rooms.themes[i];
    return NO_THEME;
}

static const char* themestr(const Map map, const Point where)
{
    return t_name(lutheme(map, where));
}

Theme m_theme(const Theme last, const Map map, const Point where, const Timer tm)
{
    const Theme now = lutheme(map, where);
    if(now != last && now != NO_THEME)
        t_set(tm.renders, tm.renders + 120, false, themestr(map, where));
    return now;
}

int m_isportal(char** block, const Point where)
{
    return p_block(where, block) == '~';
}

int m_out(const Map map, const Point where)
{
    return (int) where.x >= map.cols || (int) where.x < 0
        || (int) where.y >= map.rows || (int) where.y < 0;
}

void m_edit(const Map map, const Overview ov)
{
    if(m_out(map, ov.where))
        return;

    const int ascii = ov.selected + ' ';

    if(s_sprite(ascii))
        return;

    const int x = ov.where.x;
    const int y = ov.where.y;

    if(ov.party == FLORING) map.floring[y][x] = ascii;
    if(ov.party == WALLING) map.walling[y][x] = ascii;
    if(ov.party == CEILING) map.ceiling[y][x] = ascii;
}

void m_room(const Map map, const Point where, const int w, const int h, const Party p)
{
    for(int i = -w; i <= w; i++)
    for(int j = -h; j <= h; j++)
    {
        const int xx = where.x + i;
        const int yy = where.y + j;
        switch(p)
        {
        case WALLING: map.walling[yy][xx] = ' '; break;
        case CEILING: map.ceiling[yy][xx] = ' '; break;
        case FLORING:
            if(map.walling[yy][xx] == ' ')
                map.floring[yy][xx] = ' ';
            break;

        default:
            u_bomb("room: party not supported\n");
            break;
        }
    }
}

void m_column(const Map map, const Point where, const int ascii)
{
    const int x = where.x;
    const int y = where.y;
    map.ceiling[y][x] = map.walling[y][x] = map.floring[y][x] = ascii;
}

static void supports(const Map map, const int x, const int y, const Party p)
{
    if(p == CEILING)
    {
        map.walling[y + 1][x + 1] = map.floring[y + 1][x + 1] =
        map.walling[y + 1][x - 1] = map.floring[y + 1][x - 1] =
        map.walling[y - 1][x - 1] = map.floring[y - 1][x - 1] =
        map.walling[y - 1][x + 1] = map.floring[y - 1][x + 1] = '#';
    }
}

void m_platform(const Map map, const int x, const int y, const Party p)
{
    for(int j = -1; j <= 1; j++)
    for(int k = -1; k <= 1; k++)
    {
        const int yy = j + y;
        const int xx = k + x;
        switch(p)
        {
        case FLORING: map.floring[yy][xx] = '"'; break;
        case CEILING: map.ceiling[yy][xx] = '#'; break;
        default:
            u_bomb("platform: party not supported\n");
            break;
        }
    }
}

static void trapdoor(const Map map, const int x, const int y, const Party p)
{
    switch(p)
    {
    case FLORING: map.floring[y][x] = '~'; break;
    case CEILING: map.ceiling[y][x] = '~'; break;
    default:
        u_bomb("trapdoor: party not supported\n");
        break;
    }
}

void m_trapdoors(const Map map, const Points trapdoors, const Party p)
{
    for(int i = 0; i < trapdoors.count; i++)
    {
        const Point where = trapdoors.point[i];

        const int x = where.x;
        const int y = where.y;

        m_platform(map, x, y, p);
        supports(map, x, y, p);
        trapdoor(map, x, y, p);
    }
}

void m_corridor(const Map map, const Point a, const Point b)
{
    const Point step = p_sub(b, a);
    const Point delta = {
        step.x > 0.0f ? 1.0f : step.x < 0.0f ? -1.0f : 0.0f,
        step.y > 0.0f ? 1.0f : step.y < 0.0f ? -1.0f : 0.0f,
    };

    const int sx = fabsf(step.x);
    const int sy = fabsf(step.y);

    const int dx = delta.x;
    const int dy = delta.y;

    int x = a.x;
    int y = a.y;
    for(int i = 0; i < sx; i++) map.walling[y][x += dx] = ' ';
    for(int i = 0; i < sy; i++) map.walling[y += dy][x] = ' ';
}

static void cross(const Map map, const int x, const int y, const char a, const char b)
{
    const int end = map.grid / 2;
    if(map.walling[y + 0][x + end] == a) map.walling[y + 0][x + end] = b;
    if(map.walling[y + 0][x - end] == a) map.walling[y + 0][x - end] = b;
    if(map.walling[y + end][x + 0] == a) map.walling[y + end][x + 0] = b;
    if(map.walling[y - end][x + 0] == a) map.walling[y - end][x + 0] = b;
}

static void door(const Map map, const int x, const int y)
{
    cross(map, x, y, ' ', '!');
}

static void pass(const Map map, const int x, const int y)
{
    cross(map, x, y, '!', ' ');
}

void m_barricade(const Map map)
{
    for(int i = 0; i < map.rooms.count; i++)
    {
        const Point mid = map.rooms.wheres[i];
        door(map, mid.x, mid.y);
    }
    for(int i = 0; i < map.rooms.count; i++)
    {
        const Point mid = map.rooms.wheres[i];

        if(map.rooms.agents[i] == 0)
            pass(map, mid.x, mid.y);
    }
}

int m_min(const Map map)
{
    return map.grid / 8;
}

int m_max(const Map map)
{
    return map.grid / 2;
}

static void grass(const Map map, const Point where)
{
    const int x = where.x;
    const int y = where.y;
    for(int i = -m_max(map); i <= m_max(map); i++)
    for(int j = -m_max(map); j <= m_max(map); j++)
    {
        const int xx = x + i;
        const int yy = y + j;
        map.floring[yy][xx] = '(';
    }
}

static void path(const Map map, const Point where)
{
    const int x = where.x;
    const int y = where.y;
    const int end = m_max(map);
    if(map.walling[y + 0][x + end] == ' ') for(int i = 0; i <= end; i++) map.floring[y + 0][x + i] = '"';
    if(map.walling[y + 0][x - end] == ' ') for(int i = 0; i <= end; i++) map.floring[y + 0][x - i] = '"';
    if(map.walling[y + end][x + 0] == ' ') for(int i = 0; i <= end; i++) map.floring[y + i][x + 0] = '"';
    if(map.walling[y - end][x + 0] == ' ') for(int i = 0; i <= end; i++) map.floring[y - i][x + 0] = '"';
}

void m_themeate(const Map map)
{
    for(int i = 0; i < map.rooms.count; i++)
    {
        const Point where = map.rooms.wheres[i];
        switch(map.rooms.themes[i])
        {
        case NICE_GARDEN:
            grass(map, where);
            path(map, where);
            break;

        case WATER_WELL:
            m_room(map, where, m_min(map), m_min(map), FLORING);
            m_platform(map, where.x, where.y, FLORING);
            break;

        default:
            break;
        }
    }
}
