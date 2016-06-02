#include <iostream>

#include "game.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>
#include <cmath>

#include "tower.h"

SoMTD::Tower::Tower(std::string texture_name, unsigned id, int x, int y, std::string image_selected, Player *p) :
    m_image_path(texture_name),
    m_id(id),
    m_x(x),
    m_y(y),
    m_start(-1),
    m_priority(0),
    m_imageselected_path(image_selected),
    m_player(p)
{
    m_range = 200.0;
    m_texture = ijengine::resources::get_texture(texture_name);
    ijengine::event::register_listener(this);
}

SoMTD::Tower::~Tower()
{
    ijengine::event::unregister_listener(this);
}

bool
SoMTD::Tower::on_event(const ijengine::GameEvent& event)
{
    if (event.id() == SoMTD::UPGRADE_TOWER) {
        if (m_selected)
            m_x += 1;
    }
    if (event.id() == SoMTD::MOUSEOVER) {
        double x_pos = event.get_property<double>("x");
        double y_pos = event.get_property<double>("y");
        if (x_pos >= (canvas_x+m_texture->w()/4) && (x_pos<canvas_x+m_texture->w()-m_texture->w()/4) && (y_pos>canvas_y+m_texture->h()/4) && y_pos<(canvas_y+m_texture->h()-m_texture->h()/4)) {
            m_mouseover = true;
        } else {
            m_mouseover = false;
        }
    }

    if (event.id() == SoMTD::CLICK) {
        double x_pos = event.get_property<double>("x");
        double y_pos = event.get_property<double>("y");
        if (x_pos >= (canvas_x+m_texture->w()/4) && (x_pos<canvas_x+m_texture->w()-m_texture->w()/4) && (y_pos>canvas_y+m_texture->h()/4) && y_pos<(canvas_y+m_texture->h()-m_texture->h()/4)) {
            m_selected = true;
            m_texture = ijengine::resources::get_texture(m_imageselected_path);
            m_player->state = SoMTD::Player::PlayerState::SELECTED_TOWER;
            m_player->selected_object = this;
        } else {
            m_selected = false;
            m_texture = ijengine::resources::get_texture(m_image_path);
        }
    }

    return false;
}

void
SoMTD::Tower::draw_self(ijengine::Canvas *canvas, unsigned, unsigned)
{
    const int myw = 100;
    const int myh = 81;

    std::pair<int, int> p = SoMTD::tools::grid_to_isometric(m_x, m_y, myw, myh, 1024/2, 11);

    canvas->draw(m_texture.get(), p.first + 22, p.second - myh/2);
    if (m_mouseover) {
        for (double theta=0.0; theta < 360; ++theta) {
            double myx = ( (m_range * cos(theta)) + canvas_x + m_texture->w()/2 );
            double myy = ( m_range * sin(theta) + canvas_y + m_texture->h()/2);
            ijengine::Point myp(myx, myy);
            canvas->draw(myp);
        }
    }
}

void
SoMTD::Tower::update_self(unsigned, unsigned)
{
}
