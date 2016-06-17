#include "movable_unit.h"

#include "game.h"
#include <list>
#include <algorithm>

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/texture.h>

SoMTD::MovableUnit::MovableUnit(
        std::pair<int, int> s_pos,
        std::pair<int, int> e_pos,
        std::string t_path,
        std::vector< std::pair<int, int> > best_path,
        Player* myp,
        Animation::StateStyle entity_state_style,
        int frame_per_state,
        int total_states) :
    m_enemy(false),
    end_position(e_pos),
    start_position(s_pos),
    m_texture(ijengine::resources::get_texture(t_path)),
    m_active(false),
    m_current_instruction(0),
    m_player(myp),
    m_state_style(entity_state_style),
    m_frame_per_state(frame_per_state),
    m_total_states(total_states)
{
    m_movement_speed = std::make_pair(0.0, 0.0);
    m_labyrinth_path = best_path;
    m_animation = new Animation(s_pos.first, s_pos.second, t_path, entity_state_style, m_frame_per_state, total_states);
    std::pair<int, int> p = SoMTD::tools::grid_to_isometric(s_pos.first, s_pos.second, 100, 81, 1024/2, 11);
    desired_place = start_position;
    m_x = p.first;
    m_y = p.second;
    ijengine::event::register_listener(this);
    texture_name = t_path;
}

SoMTD::MovableUnit::~MovableUnit()
{
    ijengine::event::unregister_listener(this);
}

int
SoMTD::MovableUnit::x() const
{
    return m_x;
}

int
SoMTD::MovableUnit::y() const
{
    return m_y;
}

void
SoMTD::MovableUnit::update_self(unsigned a1, unsigned a2)
{
    if (m_active) {
        m_animation->update_screen_position(std::make_pair(m_x, m_y));
        if ((a1 % 50) == 0)
            m_animation->next_frame();

        if (m_moving) {
            double deltax = m_movement_speed.first * (a1-a2)/1000.0;
            double deltay = m_movement_speed.second * (a1-a2)/1000.0;
            printf("deltax: %f, deltay: %f\n", deltax, deltay);
            // printf("desired y: %d, actual y: %d\n", desired_place.second, m_y);

            if ((m_x+deltax*2 >= desired_place.first) && (m_x-deltax*2 <= desired_place.first) && (m_y-deltay*2 >= desired_place.second) && (m_y+deltay*2 <= desired_place.second)) {
                // printf("cheguei no destino..\n");
                m_moving = false;
                m_current_instruction++;
                if (m_current_instruction > m_labyrinth_path.size()) {
                    m_active = false;
                }
                m_movement_speed.first = 0;
                m_movement_speed.second = 0;
            }

            if (m_x+deltax >= desired_place.first && m_x-deltax <= desired_place.first) {
                m_movement_speed.first = 0;
            }
            printf("m_x: %d, m_y: %d, des_x: %d, des_y: %d\n", m_x, m_y, desired_place.first, desired_place.second);

            if (m_y-deltay*2 >= desired_place.second && m_y+deltay*2 <= desired_place.second) {
                // printf("not in y..\n");
                m_movement_speed.second = 0;
            } else {
                // printf("in y.\n");
            }

            m_y = (double)m_y + deltay;
            m_x = (double)m_x + deltax;
        } else {
            if (m_current_instruction == m_labyrinth_path.size()) {
                die();
            } else {
                std::pair<int, int> pos = m_labyrinth_path[m_current_instruction];
                move(pos.first, pos.second);
            }
        }
    }
}

void
SoMTD::MovableUnit::die()
{
    printf("unit dead!!\n");
    m_active = false;
    m_player->m_hp -= 1;
}

bool
SoMTD::MovableUnit::on_event(const ijengine::GameEvent&)
{
    return false;
}

void
SoMTD::MovableUnit::draw_self(ijengine::Canvas *c, unsigned a1, unsigned a2)
{
    if (m_active) {
        m_animation->draw(c, a1, a2);
    }
}

void
SoMTD::MovableUnit::draw_self_after(ijengine::Canvas *c, unsigned, unsigned)
{
    int half_texture = m_texture->w()/2;
    ijengine::Rectangle rect(m_x, m_y, 60*hp_percentage()/200, 5);
    c->draw(rect);
}

int
SoMTD::MovableUnit::hp_percentage() const
{
    return 100 * m_actual_hp/m_initial_hp;
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
    m_moving = false;
    m_current_instruction = 0;
    std::pair<int, int> pos = SoMTD::tools::grid_to_isometric(start_position.first, start_position.second, 100, 81, 1024/2, 11);
    m_x = pos.first;
    m_y = pos.second;
}

bool
SoMTD::MovableUnit::active() const
{
    return m_active;
}

void
SoMTD::MovableUnit::move(int x, int y)
{
    printf("x: %d, y: %d\n", x, y);
    m_moving = true;
    const int tile_width = 100;
    const int tile_height = 81;
    desired_place = SoMTD::tools::grid_to_isometric(x, y, tile_width, tile_height, 1024/2, 11);
    printf("actual place: [%d][%d]\n", m_y, m_x);
    printf("desired place: [%d][%d]\n", desired_place.second, desired_place.first);

    if (desired_place.first < m_x) {
        m_movement_speed.first = -400.0;
    } else if (desired_place.first > m_x)
        m_movement_speed.first = 400.0;
    else
        m_movement_speed.first = 0.0;

    if (desired_place.second < m_y) {
        m_movement_speed.second = -400.0;
    } else if (desired_place.second > m_y)
        m_movement_speed.second = 400.0;
    else
        m_movement_speed.second  = 0.0;
}

SoMTD::MovableUnit*
SoMTD::MovableUnit::clone()
{
    return new MovableUnit(start_position, end_position, texture_name, m_labyrinth_path, m_player, m_state_style, m_frame_per_state, m_total_states);
}
