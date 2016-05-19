#include <iostream>

#include "game.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>

#include "panel.h"

SoMTD::Panel::Panel(std::string texture_name, unsigned id, int x, int y) :
    m_id(id),
    m_x(x),
    m_y(y),
    m_start(-1),
    m_priority(100000)
{
    m_texture = ijengine::resources::get_texture(texture_name);
    ijengine::event::register_listener(this);
}

SoMTD::Panel::~Panel()
{
    ijengine::event::unregister_listener(this);
}

bool
SoMTD::Panel::on_event(const ijengine::GameEvent&)
{
    return false;
}

void
SoMTD::Panel::draw_self(ijengine::Canvas *canvas, unsigned, unsigned)
{
    canvas->draw(m_texture.get(), m_x, m_y);
}

void
SoMTD::Panel::update_self(unsigned, unsigned)
{
}
