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
        Animation::StateStyle statestyle, int frame_per_state, int total_states, float newattackspeed,
        int newdamage) :
    m_image_path(texture_name),
    m_id(id),
    m_start(-1),
    m_priority(0),
    m_imageselected_path(image_selected),
    m_player(p)
{
    m_attack_speed = newattackspeed;
    m_damage = newdamage;
    m_level = 1;
    m_range = 85.0;
    m_texture = ijengine::resources::get_texture(texture_name);
    m_animation = new Animation(x, y, texture_name, statestyle, frame_per_state, total_states);
    ijengine::event::register_listener(this);
    mytimer = 0;
    m_cooldown = 0;
    m_actual_state = IDLE;
    m_projectiles = new std::list<Projectile*>();
}

SoMTD::Tower::~Tower()
{
    delete m_projectiles;
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

    for (auto it=m_projectiles->begin(); it != m_projectiles->end(); ++it) {
        (*it)->draw_self(canvas, a1, a2);
    }

    if (m_mouseover) {
        std::pair<int, int> pos = m_animation->screen_position();
        int half_h = m_animation->height()/2;
        for (double theta=0.0; theta < 360; ++theta) {
            double myx = ( (m_range * cos(theta)) + pos.first + m_animation->width()-15);
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

    for (auto it=m_projectiles->begin(); it != m_projectiles->end(); ++it) {
        if ((*it)->done())
            it = m_projectiles->erase(it);
        else
            (*it)->update_self(now, last);
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
SoMTD::Tower::draw_self_after(ijengine::Canvas* c, unsigned a1, unsigned a2)
{
    for (auto it=m_projectiles->begin(); it != m_projectiles->end(); ++it) {
        (*it)->draw_self_after(c, a1, a2);
    }

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
SoMTD::Tower::handle_idle_state(unsigned, unsigned)
{
}

void
SoMTD::Tower::handle_attacking_state(unsigned now, unsigned)
{
    if (now > m_cooldown) {
        if (m_target) {
            if (m_target->active()) {
                double dx = animation()->screen_position().first - target()->animation()->screen_position().first;
                double dy = animation()->screen_position().second - target()->animation()->screen_position().second;
                double distance = sqrt(dx*dx + dy*dy);
                if (distance < range()+target()->animation()->width()/2) {
                    Projectile* p = new Projectile(target(), std::make_pair(target()->animation()->screen_position().first, target()->animation()->screen_position().second), "projectiles/projetil_caveira.png", std::make_pair(animation()->screen_position().first, animation()->screen_position().second), 1, 1, damage());
                    m_projectiles->push_back(p);
                    m_cooldown = now+1000*attack_speed();
                    if (m_id == 0x001)
                        m_player->increase_gold(damage());
                } else {
                    m_actual_state = SoMTD::Tower::IDLE;
                    m_target = nullptr;
                }
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
    // If it is a poseidon tower, it can have multiple targets
    switch (id()) {
        // poseidon towers
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            if (m_cooldown < now) {
                m_cooldown = now+attack_speed()*1000;
                m_actual_state = IDLE;

                /* pushing an slow event to the queue of events. the first
                 * argument is the x_position of the unit, the second argument
                 * is the y_position of the unit, the third argument is the
                 * range of the slow, 4th argument is the dmg of the slow,
                 * 5th argument is the slow coefficient, and 6th the time penalization */
                player()->units_events()->push_back(0x000);
                player()->event_args()->push_back((int)newtarget->x());
                player()->event_args()->push_back((int)newtarget->y());
                player()->event_args()->push_back(50);
                player()->event_args()->push_back(damage());
                player()->event_args()->push_back(500);
                player()->event_args()->push_back(2000);
            }
            break;

        //zeus towers
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
            if (m_cooldown < now) {
                m_cooldown = now+attack_speed()*1000;
                m_target = newtarget;
                Projectile* p = new Projectile(target(), std::make_pair(target()->animation()->screen_position().first, target()->animation()->screen_position().second), "projectiles/projetil_zeus2.png", std::make_pair(animation()->screen_position().first, animation()->screen_position().second), 1, 1, damage());
                m_projectiles->push_back(p);
                m_actual_state = State::ATTACKING;
                m_player->increase_gold(damage());
            }
            break;

         //hades towers
         case 0x102:
             if (m_cooldown < now) {
                 m_cooldown = now+attack_speed()*1000;
                 m_target = newtarget;
                 m_actual_state = State::ATTACKING;
                 Projectile* p = new Projectile(target(), std::make_pair(target()->animation()->screen_position().first, target()->animation()->screen_position().second), "projectiles/projetil_caveira.png", std::make_pair(animation()->screen_position().first, animation()->screen_position().second), 1, 1, damage());
                 m_projectiles->push_back(p);
                 newtarget->suffer_poison(damage()*5, 10000, now, last);
             }
             break;

        case 0x103:
            if (m_cooldown < now) {
                m_cooldown = now+attack_speed()*1000;
                m_target = newtarget;
                m_actual_state = State::ATTACKING;
                Projectile* p = new Projectile(target(), std::make_pair(target()->animation()->screen_position().first, target()->animation()->screen_position().second), "projectiles/projetil_caveira.png", std::make_pair(animation()->screen_position().first, animation()->screen_position().second), 1, 1, damage());
                m_projectiles->push_back(p);
                newtarget->suffer_bleed(damage(), 10000, now, last);
            }
            break;

        default:
            m_cooldown = now+attack_speed()*1000;
            m_target = newtarget;
            Projectile* p = new Projectile(target(), std::make_pair(target()->animation()->screen_position().first, target()->animation()->screen_position().second), "projectiles/projetil_caveira.png", std::make_pair(animation()->screen_position().first, animation()->screen_position().second), 1, 1, damage());
            m_projectiles->push_back(p);
            m_actual_state = State::ATTACKING;
            break;
    }
}

SoMTD::MovableUnit*
SoMTD::Tower::target() const
{
    return m_target;
}

double
SoMTD::Tower::attack_speed() const
{
    return m_attack_speed;
}

unsigned
SoMTD::Tower::id() const
{
    return m_id;
}

SoMTD::Player*
SoMTD::Tower::player() const
{
    return m_player;
}
