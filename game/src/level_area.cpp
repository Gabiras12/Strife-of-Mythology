#include <iostream>

#include "game.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>

#include "level_area.h"
#include "translator.h"

SoMTD::LevelArea::LevelArea(std::string texture_name, unsigned id, int x, int y, int p) :
    m_texture(ijengine::resources::get_texture(texture_name)),
    m_id(id),
    m_x(x),
    m_y(y),
    m_priority(p)
{
    m_start = -1;
    ijengine::event::register_listener(this);
}

SoMTD::LevelArea::~LevelArea()
{
    ijengine::event::unregister_listener(this);
}

bool
SoMTD::LevelArea::on_event(const ijengine::GameEvent&)
{
    return false;
}

void
SoMTD::LevelArea::draw_self(ijengine::Canvas *canvas, unsigned, unsigned)
{
    int myw = m_texture->w();
    int myh = m_texture->h();

    std::pair<int, int> p = screen_coordinates(m_x, m_y, myw/2, myh/2);
    int x_pos = p.first;
    int y_pos = p.second;

    // x0 = half of window width, the coeficient for the isometry
    int x0 = 1024/2;
    const int block_offset_region = 11;
    canvas->draw(m_texture.get(), x_pos+x0 - m_texture->w()/2, y_pos-(block_offset_region*m_y)-(block_offset_region*m_x));
}

void
SoMTD::LevelArea::update_self(unsigned, unsigned)
{
}

std::pair<int, int>
SoMTD::LevelArea::screen_coordinates(int map_x, int map_y, int tw, int th)
{
    int xs = (map_x - map_y) * tw;
    int ys = (map_x + map_y) * th;;
    return std::pair<int, int>(xs, ys);
}

