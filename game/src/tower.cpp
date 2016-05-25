#include <iostream>

#include "game.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>
#include <cmath>

#include "tower.h"

SoMTD::Tower::Tower(std::string texture_name, unsigned id, int x, int y) :
    m_id(id),
    m_x(x),
    m_y(y),
    m_start(-1),
    m_priority(0)
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
    if (event.id() == SoMTD::MOUSEOVER) {
        double x_pos = event.get_property<double>("x");
        double y_pos = event.get_property<double>("y");
        if (x_pos >= canvas_x && x_pos<canvas_x+m_texture->w() && y_pos>canvas_y && y_pos<canvas_y+m_texture->h()) {
            m_mouseover = true;
        } else {
            m_mouseover = false;
        }
    }

    return false;
}

void
SoMTD::Tower::draw_self(ijengine::Canvas *canvas, unsigned, unsigned)
{
    const int myw = 100;
    const int myh = 81;

    std::pair<int, int> p = screen_coordinates(m_x, m_y, myw, myh);
    int x_pos = p.first;
    int y_pos = p.second;
    // printf("x_pos: %d, y_pos: %d\n", x_pos, y_pos);

    // x0 = half of window width, the coeficient for the isometry
    int x0 = 1024/2;
    // canvas->draw(m_texture.get(), x_pos+x0-((m_y+m_x)*11), y_pos-(11*(m_x-m_y)));

    canvas_x = x_pos+x0 - myw/2;
    canvas_y = -myh/2 +y_pos-11*(m_y+m_x);
    canvas->draw(m_texture.get(), x_pos+x0 - myw/2, - myh/2 +y_pos-11*(m_y+m_x));
    if (m_mouseover) {
        for (double theta=0.0; theta < 360; ++theta) {
            double myx = ( (m_range * cos(theta)) + canvas_x + m_texture->w()/2 );
            double myy = ( m_range * sin(theta) + canvas_y + m_texture->h()/2);
            ijengine::Point myp(myx, myy);
            canvas->draw(myp);

        }
        // ijengine::Rectangle myrect {canvas_x, canvas_y, m_texture->w(), m_texture->h() };
        // canvas->draw(myrect);
    }
    // canvas->draw
    // canvas->draw(m_texture.get(), x_pos + x0 - myw/2, y_pos - myh/2);
}

void
SoMTD::Tower::update_self(unsigned, unsigned)
{
}

std::pair<int, int>
SoMTD::Tower::screen_coordinates(int map_x, int map_y, int tw, int th)
{
    int xs = (map_x - map_y) * (tw / 2);
    int ys = (map_x + map_y) * (th / 2);

    return std::pair<int, int>(xs, ys);
}

