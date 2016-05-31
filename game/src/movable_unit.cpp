#include "movable_unit.h"

#include "game.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>

SoMTD::MovableUnit::MovableUnit(std::pair<int, int> s_pos, std::pair<int, int> e_pos, std::string t_path) :
    m_enemy(false),
    end_position(e_pos),
    start_position(s_pos),
    m_texture(ijengine::resources::get_texture(t_path)),
    m_active(false)
{
    ijengine::event::register_listener(this);
}

SoMTD::MovableUnit::~MovableUnit()
{
    ijengine::event::unregister_listener(this);
}

void
SoMTD::MovableUnit::update_self(unsigned start, unsigned end)
{
    if ((start % 1000) == 0) {
        spawn();
    }
}

bool
SoMTD::MovableUnit::on_event(const ijengine::GameEvent& event)
{
    return false;
}

void
SoMTD::MovableUnit::draw_self(ijengine::Canvas *c, unsigned, unsigned)
{
    if (m_active)
        c->draw(m_texture.get(), 5, 5);
}

void
SoMTD::MovableUnit::draw_self_after(ijengine::Canvas *c, unsigned, unsigned)
{

}

bool
SoMTD::MovableUnit::enemy() const
{
    return m_enemy;
}

void
SoMTD::MovableUnit::spawn()
{
    m_active = true;
}

bool
SoMTD::MovableUnit::active() const
{
    return m_active;
}
