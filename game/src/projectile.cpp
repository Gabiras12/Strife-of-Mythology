#include "projectile.h"
#include <iostream>

SoMTD::Projectile::Projectile(MovableUnit *newtarget, std::pair<double, double> to, std::string texture, std::pair<double, double> origin,
        int new_frame_per_state, int new_total_states, double newdamage) :
    m_target(newtarget),
    m_destination(to),
    m_texture_path(texture),
    m_x(origin.first),
    m_y(origin.second),
    m_done(false),
    m_damage(newdamage)
{
    m_animation = new Animation(m_x, m_y, texture, SoMTD::Animation::StateStyle::EVERYTHING_PER_LINE, new_frame_per_state, new_total_states);
    m_movement_speed = std::make_pair((to.first - m_x)/100.0, (to.second - m_y)/100.0);
}

SoMTD::Projectile::~Projectile()
{
    delete m_animation;
}

void
SoMTD::Projectile::draw_self(ijengine::Canvas *canvas, unsigned a1, unsigned a2)
{
    if (not m_done)
        m_animation->draw(canvas, a1, a2);
}

void
SoMTD::Projectile::draw_self_after(ijengine::Canvas *canvas, unsigned a1, unsigned a2)
{
    if (not m_done)
        m_animation->draw_self_after(canvas, a1, a2);
}

void
SoMTD::Projectile::update_self(unsigned, unsigned)
{
    if (m_x + 10 > m_destination.first && m_x - 10 < m_destination.first && m_y + 10 > m_destination.second && m_y - 10 < m_destination.second) {
        m_target->suffer(damage());
        m_done = true;
    }

    if (not m_done) {
        m_x += m_movement_speed.first;
        m_y += m_movement_speed.second;
        m_animation->update_tile(std::make_pair((int)m_x, (int)m_y));
    }
}

SoMTD::Animation*
SoMTD::Projectile::animation() const
{
    return m_animation;
}

bool
SoMTD::Projectile::done() const
{
    return m_done;
}

double
SoMTD::Projectile::damage() const
{
    return m_damage;
}
