#include <iostream>
#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>
#include <ijengine/rectangle.h>
#include <cmath>

#include "game.h"
#include "tower.h"
#include "animation.h"

SoMTD::Tower::Tower(std::string texture_name, unsigned id, int x, int y, std::string image_selected, Player *p,
        Animation::StateStyle statestyle, int frame_per_state, int total_states) :
    m_image_path(texture_name),
    m_id(id),
    m_start(-1),
    m_priority(0),
    m_imageselected_path(image_selected),
    m_player(p)
{
    m_damage = 10;
    m_level = 1;
    m_range = 50.0;
    m_texture = ijengine::resources::get_texture(texture_name);
    m_animation = new Animation(x, y, texture_name, statestyle, frame_per_state, total_states);
    ijengine::event::register_listener(this);
    mytimer = 0;
    m_cooldown = 0;
    m_actual_state = IDLE;
}

SoMTD::Tower::~Tower()
{
    ijengine::event::unregister_listener(this);
}

bool
SoMTD::Tower::on_event(const ijengine::GameEvent& event)
{
    if (event.id() == SoMTD::UPGRADE_TOWER) {
        if (m_selected && m_player->gold() > 250){
            m_player->discount_gold(250);
            level_up();
        }
    }

    std::pair<int, int> screen_pos = m_animation->screen_position();

    if (event.id() == SoMTD::MOUSEOVER) {
        double x_pos = event.get_property<double>("x");
        double y_pos = event.get_property<double>("y");
        std::pair<int, int> click_as_tile = SoMTD::tools::isometric_to_grid(x_pos, y_pos, 100, 81, 1024/2, 11);
        if (click_as_tile.first == m_animation->tile().first && click_as_tile.second == m_animation->tile().second) {
            m_mouseover = true;
        } else {
            m_mouseover = false;
        }
    }

    if (event.id() == SoMTD::CLICK) {
        double x_pos = event.get_property<double>("x");
        double y_pos = event.get_property<double>("y");
        std::pair<int, int> click_as_tile = SoMTD::tools::isometric_to_grid(x_pos, y_pos, 100, 81, 1024/2, 11);
        if (click_as_tile.first == m_animation->tile().first && click_as_tile.second == m_animation->tile().second) {
            m_selected = true;
            m_animation->update_texture(m_imageselected_path);
            m_player->state = SoMTD::Player::PlayerState::SELECTED_TOWER;
            m_player->selected_object = this;
        } else {
            m_selected = false;
            m_animation->update_texture(m_image_path);
        }
    }

    return false;
}

void
SoMTD::Tower::draw_self(ijengine::Canvas *canvas, unsigned a1, unsigned a2)
{
    m_animation->draw(canvas, a1, a2);
    if (m_mouseover) {
        std::pair<int, int> pos = m_animation->screen_position();
        int half_w = m_animation->width()/2;
        int half_h = m_animation->height()/2;
        for (double theta=0.0; theta < 360; ++theta) {
            double myx = ( (m_range * cos(theta)) + pos.first + m_animation->width() );
            double myy = ( m_range * sin(theta) + pos.second + half_h/2);
            ijengine::Point myp(myx, myy);
            canvas->draw(myp);
        }
    }
}

void
SoMTD::Tower::update_self(unsigned now, unsigned last)
{
    if (m_next_frame_time == 0) {
        m_next_frame_time = now+(1000/m_animation->frame_per_state());
    }

    if (now > m_next_frame_time) {
        m_animation->next_frame();
        m_next_frame_time += 1000/m_animation->frame_per_state();
    }
    switch (actual_state()) {
        case IDLE:
            handle_idle_state(now, last);
        break;

        case ATTACKING:
            handle_attacking_state(now, last);
        break;

        default:
        break;
    }
}

void
SoMTD::Tower::level_up()
{
    m_level+=1;
    m_damage *=1.15;
    m_range+=30.0;
}

int
SoMTD::Tower::level() const
{
    return m_level;
}

int
SoMTD::Tower::damage() const
{
    return m_damage;
}

double
SoMTD::Tower::range() const
{
    return m_range;
}

void
SoMTD::Tower::draw_self_after(ijengine::Canvas* c, unsigned, unsigned)
{
    if (m_selected) {
        std::pair<int, int> pos = m_animation->screen_position();
        auto font = ijengine::resources::get_font("Forelle.ttf", 40);
        c->set_font(font);
        c->draw("Press U", pos.first, pos.second-80);
    }
}


SoMTD::Animation*
SoMTD::Tower::animation() const
{
    return m_animation;
}

SoMTD::Tower::State
SoMTD::Tower::actual_state() const
{
    return m_actual_state;
}

void
SoMTD::Tower::handle_idle_state(unsigned now, unsigned last)
{
}

void
SoMTD::Tower::handle_attacking_state(unsigned now, unsigned last)
{
    if (now > m_cooldown) {
        if (m_target) {
            if (m_target->active()) {
                m_target->suffer(damage());
                m_cooldown = now+1000;
            } else {
                m_actual_state = SoMTD::Tower::IDLE;
                m_target = nullptr;
            }
        } else {
            m_actual_state = SoMTD::Tower::IDLE;
        }
    }
}

void
SoMTD::Tower::attack(SoMTD::MovableUnit* newtarget, unsigned now, unsigned last)
{
    m_target = newtarget;
    m_cooldown = now+1000;
    newtarget->suffer(damage());
    m_actual_state = SoMTD::Tower::State::ATTACKING;
}
