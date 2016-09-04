#ifndef SOMTD_TOWER_H
#define SOMTD_TOWER_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>

#include <memory>
#include "player.h"
#include "animation.h"
#include "movable_unit.h"
#include "projectile.h"
#include "attack.h"

/* IDS FOR TOWERS:
 * 0x0000 = zeus tower
 * 0x0010 = poseidon tower
 * 0x0100 = hades tower
 */

namespace SoMTD {
    class Tower : public ijengine::GameObject, public ijengine::GameEventsListener {

    public:
        Tower(std::string texture_name, unsigned id, int x, int y, std::string m_imageselected_path, Player *p,
                Animation::StateStyle state_style, int frame_per_state, int total_states, float attackspeed, int newdamage);
        ~Tower();

        enum State {
            IDLE = 0x00,
            ATTACKING = 0x01
        };

        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        void draw_self_after(ijengine::Canvas *c, unsigned now, unsigned last);
        bool on_event(const ijengine::GameEvent& event);
        void update_self(unsigned, unsigned);
        int level() const;
        // void attack(SoMTD::MovableUnit* newtarget, unsigned now, unsigned last);
        double range() const;
        SoMTD::Animation *animation() const;
        unsigned mytimer;
        Tower::State actual_state() const;
        int damage() const;
        SoMTD::MovableUnit* target() const;
        unsigned id() const;
        SoMTD::Player* player() const;
        std::list<Projectile*>* projectiles() const;
        Attack *attack() const;

    protected:
        void level_up();

    private:
        double m_range = 200.0;
        int m_level;
        int m_damage;
        std::shared_ptr<ijengine::Texture> m_texture;
        std::string m_image_path;
        unsigned m_id;
        bool m_selected = false;
        int m_start;
        int m_priority;
        bool m_mouseover = false;
        std::string m_imageselected_path;
        Player *m_player;
        Animation *m_animation;
        unsigned m_next_frame_time = 0;
        void build_tower(unsigned tower_id);
        Tower::State m_actual_state;
        void handle_idle_state(unsigned now, unsigned last);
        void handle_attacking_state(unsigned now, unsigned last);
        SoMTD::MovableUnit* m_target;
        unsigned m_cooldown;
        std::list<Projectile*> *m_projectiles;
        Attack *m_attack;
    };
}

#endif
