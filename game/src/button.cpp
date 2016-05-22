#include <iostream>

#include "game.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>

#include "button.h"

SoMTD::Button::Button(std::string texture_name, unsigned id, int x, int y, std::string mtexture) :
    m_texture(ijengine::resources::get_texture(texture_name)),
    m_id(id),
    m_x(x),
    m_y(y),
    m_mouseover_texture(ijengine::resources::get_texture(mtexture))
{
    m_mouseover = false;
    ijengine::event::register_listener(this);
}

SoMTD::Button::~Button()
{
    ijengine::event::unregister_listener(this);
}

void
SoMTD::Button::update_self(unsigned, unsigned)
{
}


bool
SoMTD::Button::on_event(const ijengine::GameEvent& event)
{

    if (event.id() == SoMTD::MOUSEOVER) {
        double x_pos = event.get_property<double>("x");
        double y_pos = event.get_property<double>("y");
        if (x_pos >= m_x && x_pos<m_x+m_texture->w() && y_pos>m_y && y_pos<m_y+m_texture->h()) {
            m_mouseover = true;
        } else {
            m_mouseover = false;
        }
    }
    return false;
}

void
SoMTD::Button::draw_self(ijengine::Canvas *c, unsigned, unsigned)
{
    if (m_mouseover) {
        c->draw(m_mouseover_texture.get(), m_x, m_y);
    } else  {
        c->draw(m_texture.get(), m_x, m_y);
    }
}
