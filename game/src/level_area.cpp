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

    std::pair<int, int> p = SoMTD::tools::grid_to_isometric(m_x, m_y, myw, myh, 1024/2, 11);
    canvas->draw(m_texture.get(), p.first, p.second);
}

void
SoMTD::LevelArea::draw_self_after(ijengine::Canvas *canvas, unsigned, unsigned)
{
    int myw = m_texture->w();
    int myh = m_texture->h();
    if ((m_id % 10) == 0) {
        std::pair<int, int> p = SoMTD::tools::grid_to_isometric(m_x, m_y, myw, myh, 1024/2, 11);
        canvas->draw(ijengine::resources::get_texture("start.png").get(), p.first+(80-myw/2), p.second+81/2-myh);
    } else if ((m_id == 9 || m_id == 19 || m_id == 29)) {
        std::pair<int, int> p = SoMTD::tools::grid_to_isometric(m_x, m_y, myw, myh, 1024/2, 11);
        canvas->draw(ijengine::resources::get_texture("destination.png").get(), p.first+(80-myw/2), p.second+81/2-myh);
    }
}

void
SoMTD::LevelArea::update_self(unsigned, unsigned)
{
}

