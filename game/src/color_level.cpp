#include "color_level.h"
#include "rectangle.h"

#include <ijengine/engine.h>
#include <ijengine/canvas.h>

#include <vector>

using namespace std;
using namespace ijengine;

ColorLevel::ColorLevel(int r, int g, int b, const string& next_level)
    : m_r(r), m_g(g), m_b(b), m_done(false), m_next(next_level), m_start(-1),
    m_texture(nullptr)
{
}

bool
ColorLevel::done() const
{
    return m_done;
}

string
ColorLevel::next() const
{
    return m_next;
}

void
ColorLevel::update_self(unsigned now, unsigned)
{
    if (m_start == -1)
        m_start = now;

    if (now - m_start > 2000)
        m_done = true;
}

void
ColorLevel::draw_self(Canvas *canvas, unsigned, unsigned)
{
    if (not m_texture)
        m_texture = resources::get_texture(canvas, "slopeE.png");

    vector< shared_ptr<Texture> > textures;

    textures.push_back(resources::get_texture(canvas, "slopeE.png"));
    textures.push_back(resources::get_texture(canvas, "slopeN.png"));
    textures.push_back(resources::get_texture(canvas, "slopeS.png"));
    textures.push_back(resources::get_texture(canvas, "slopeW.png"));

    canvas->clear();

    const int MAP_W = 8;
    const int MAP_H = 5;

    const int x0 = 320;
    const int y0 = 50;

    if (m_texture)
    {
        for (int i = 0; i < MAP_W; ++i)
            for (int j = 0; j < MAP_H; ++j)
            {
                auto p = screen_coordinates(i, j, m_texture->w(), m_texture->h());
                int xs = p.first;
                int ys = p.second;

                int text = (i + j) % 4;
                canvas->draw(textures[text].get(), xs + x0 - m_texture->w() / 2, ys + y0);
            }

        unsigned delay = 3000000;
        while(delay--);
    }
}

pair<int, int>
ColorLevel::screen_coordinates(int map_x, int map_y, int tw, int th)
{
    int xs = (map_x - map_y) * (tw / 2);
    int ys = (map_x + map_y) * (th / 2);

    return pair<int, int>(xs, ys);
}
