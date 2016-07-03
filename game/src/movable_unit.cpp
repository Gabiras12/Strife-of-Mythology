#include "movable_unit.h"

#include "game.h"
#include "animation.h"
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
        int total_states,
        int unit_hp,
        int unit_reward,
        int unit_time,
        int hp_discount_unit_win
        ) :
    m_enemy(true),
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
    m_status_list = new std::list<MovableUnit::Status>();
    m_time_per_tile = unit_time;
    m_initial_hp = unit_hp;
    m_actual_hp = unit_hp;
    m_hp_dicount_unit = hp_discount_unit_win;
    m_gold_award = unit_reward;
    m_done = false;
    m_movement_speed = std::make_pair(0.0, 0.0);
    m_labyrinth_path = best_path;
    m_animation = new Animation(s_pos.first, s_pos.second, t_path, entity_state_style, m_frame_per_state, total_states);
    std::pair<int, int> p = SoMTD::tools::grid_to_isometric(s_pos.first, s_pos.second, 100, 81, 1024/2, 11);
    start_position = s_pos;
    desired_place = start_position;
    m_x = p.first;
    m_y = p.second;
    ijengine::event::register_listener(this);
    texture_name = t_path;
    m_dead = false;
}

SoMTD::MovableUnit::~MovableUnit()
{
    delete m_status_list;
    ijengine::event::unregister_listener(this);
}

double
SoMTD::MovableUnit::x() const
{
    return m_x;
}

double
SoMTD::MovableUnit::y() const
{
    return m_y;
}

void
SoMTD::MovableUnit::update_self(unsigned now, unsigned last)
{
    if (m_next_frame < now) {
        m_next_frame = now + (1000/m_animation->frame_per_state());
        m_animation->next_frame();
    }

    if (m_active) {
        m_animation->update_screen_position(std::make_pair(m_x, m_y));
        double status_coeff = 1;
        if (m_moving) {
            if (not status_list()->empty()) {
                for (auto status=status_list()->begin(); status != status_list()->end(); ++status) {
                    switch ((int)(*status)) {
                        case SLOWED:
                            if (now > m_slow_penalization) {
                                status = status_list()->erase(status);
                            } else {
                                status_coeff = (double)m_slow_coeff/1000.0;
                            }
                            break;

                        default:
                            break;
                    }
                }
            }

            m_x = x() + m_movement_speed.first*status_coeff;
            m_y = y() + m_movement_speed.second*status_coeff;

            if (x()+1 > desired_place.first && x()-1 < desired_place.first && y()+1>desired_place.second && y()-1<desired_place.second) {
                m_moving = false;
                m_current_instruction++;
                if (m_current_instruction > m_labyrinth_path.size()) {
                    m_active = false;
                }
            }
        } else {
            if (m_current_instruction == m_labyrinth_path.size()) {
                m_player->discount_hp(m_hp_dicount_unit);
                die();
            } else {
                std::pair<int, int> pos = m_labyrinth_path[m_current_instruction];
                move(pos.first, pos.second, now);
            }
        }
    }
}

void
SoMTD::MovableUnit::die()
{
    m_active = false;
    m_done = true;
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
    ijengine::Rectangle rect(m_x+100/2, m_y-20, 100*hp_percentage()/200, 5);
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
SoMTD::MovableUnit::move(int new_x, int new_y, unsigned now)
{
    m_moving = true;
    const int tile_width = 100;
    const int tile_height = 81;
    desired_place = SoMTD::tools::grid_to_isometric(new_x, new_y, tile_width, tile_height, 1024/2, 11);
    m_movement_speed.first = desired_place.first - x();
    m_movement_speed.first /= (m_time_per_tile);
    m_movement_speed.second = (desired_place.second - y());
    m_movement_speed.second /= (m_time_per_tile);

    if (m_movement_speed.first > 0) {
        animation()->update_direction(Animation::DirectionState::DIRECTION_RIGHT);
    } else if (m_movement_speed.first < 0) {
        animation()->update_direction(Animation::DirectionState::DIRECTION_LEFT);
    }
}

SoMTD::MovableUnit*
SoMTD::MovableUnit::clone()
{
    return new MovableUnit(start_position, end_position, texture_name, m_labyrinth_path, m_player, m_state_style, m_frame_per_state, m_total_states, m_initial_hp, m_gold_award, m_time_per_tile);
}

bool
SoMTD::MovableUnit::done() const
{
    return m_done;
}

SoMTD::Animation*
SoMTD::MovableUnit::animation() const
{
    return m_animation;
}

void
SoMTD::MovableUnit::suffer(int dmg)
{
    m_actual_hp -= dmg;
    if (m_actual_hp < 1) {
        die();
        m_dead = true;
        m_player->update_gold(m_player->gold()+gold_award());
    }
}

bool
SoMTD::MovableUnit::dead() const
{
    return m_dead;
}

int
SoMTD::MovableUnit::gold_award() const
{
    return m_gold_award;
}

void
SoMTD::MovableUnit::suffer_slow(int slow_coeff, int time_penalization, unsigned now, unsigned)
{
    m_slow_coeff = slow_coeff;
    m_status_list->push_back(Status::SLOWED);
    m_slow_penalization = time_penalization + now;

}

std::list<SoMTD::MovableUnit::Status>*
SoMTD::MovableUnit::status_list() const
{
    return m_status_list;
}
