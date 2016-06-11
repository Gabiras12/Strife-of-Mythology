#include "menu_level.h"
#include "game.h"

#include <ijengine/engine.h>
#include <ijengine/canvas.h>

SoMTD::MenuLevel::MenuLevel(std::string _map_path, std::string _new_map_path, std::string p_audio) :
    m_next(_new_map_path),
    m_audio(p_audio)
{
    m_texture = ijengine::resources::get_texture("Menu.png");
    ijengine::event::register_listener(this);
}

SoMTD::MenuLevel::~MenuLevel()
{
    ijengine::event::unregister_listener(this);
}

void
SoMTD::MenuLevel::draw_self(ijengine::Canvas *c, unsigned, unsigned)
{
    c->draw(m_texture.get(), 0, 0);
}

void
SoMTD::MenuLevel::draw_self_after(ijengine::Canvas*, unsigned, unsigned)
{
}


bool
SoMTD::MenuLevel::on_event(const ijengine::GameEvent& event)
{
    if (event.id() == 777) {
        m_done = true;
        return true;
    }
    return false;
}

std::string
SoMTD::MenuLevel::audio() const
{
    return m_audio;
}

std::string
SoMTD::MenuLevel::next() const
{
    return m_next;
}

bool
SoMTD::MenuLevel::done() const
{
    return m_done;
}

void
SoMTD::MenuLevel::update_self(unsigned, unsigned)
{
}
