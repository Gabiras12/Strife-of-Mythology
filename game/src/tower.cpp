#include <iostream>
#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>
#include <cmath>

#include "game.h"
#include "tower.h"
#include "animation.h"

SoMTD::Tower::Tower(std::string texture_name, unsigned id, int x, int y, std::string image_selected, Player *p) :
    m_image_path(texture_name),
    m_id(id),
    m_start(-1),
    m_priority(0),
    m_imageselected_path(image_selected),
    m_player(p)
{
    auto pos = SoMTD::tools::grid_to_isometric(m_x, m_y, 100, 81, 1024/2, 11);
    m_range = 50.0;
    m_texture = ijengine::resources::get_texture(texture_name);
    m_animation = new Animation(x, y, texture_name, Animation::StateStyle::STATE_PER_LINE, 0x04, 0x01, 0x04);
    ijengine::event::register_listener(this);
    mytimer = 0;
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
    std::pair<int, int> tile_pos = m_animation->tile();

    if (event.id() == SoMTD::MOUSEOVER) {
        double x_pos = event.get_property<double>("x");
        double y_pos = event.get_property<double>("y");
        if (x_pos >= screen_pos.first && x_pos<screen_pos.first+m_animation->width() && (y_pos>screen_pos.second) && y_pos<(screen_pos.second+m_animation->height())) {
            m_mouseover = true;
        } else {
            m_mouseover = false;
        }
    }

    if (event.id() == SoMTD::CLICK) {
        double x_pos = event.get_property<double>("x");
        double y_pos = event.get_property<double>("y");
        if (x_pos >= screen_pos.first && x_pos<screen_pos.first+m_texture->w() && (y_pos>screen_pos.second) && y_pos<(screen_pos.second+m_texture->h())) {
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
    const int myw = 100;
    const int myh = 81;

    std::pair<int, int> p = SoMTD::tools::grid_to_isometric(m_x, m_y, myw, myh, 1024/2, 11);

    m_animation->draw(canvas, a1, a2);
    if (m_mouseover) {
        std::pair<int, int> pos = m_animation->screen_position();
        int half_w = m_animation->width()/2;
        int half_h = m_animation->height()/2;
        for (double theta=0.0; theta < 360; ++theta) {
            double myx = ( (m_range * cos(theta)) + pos.first + half_w/2 );
            double myy = ( m_range * sin(theta) + pos.second + half_h/2);
            ijengine::Point myp(myx, myy);
            canvas->draw(myp);
        }
    }
}

void
SoMTD::Tower::update_self(unsigned a1, unsigned a2)
{
    if (mytimer == 0) {
        mytimer = a1;
    } else {
        if ((a1 % 50) == 0) {
            mytimer = 0;
            m_animation->next_frame();
        }
    }
}

void
SoMTD::Tower::level_up()
{
    m_level+=1;
    m_attack*=1.15;
    m_range+=30.0;
}

int
SoMTD::Tower::level() const
{
    return m_level;
}

int
SoMTD::Tower::attack() const
{
    return m_attack;
}

double
SoMTD::Tower::range() const
{
    return m_range;
}

void
SoMTD::Tower::draw_self_after(ijengine::Canvas *c, unsigned, unsigned)
{
}

