#ifndef SOMTD_MOVABLE_UNIT_H
#define SOMTD_MOVABLE_UNIT_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>
#include <string>
#include <memory>
#include <queue>
#include <list>
#include <vector>
#include "player.h"
#include "animation.h"

namespace SoMTD {

    class MovableUnit : public ijengine::GameObject, public ijengine::GameEventsListener {
    public:
        MovableUnit(std::pair<int, int> s_pos, std::pair<int, int> e_pos, std::string texture_path, std::vector< std::pair<int, int> >, Player* playerz, Animation::StateStyle entity_state, int frame_per_state, int total_states);
        ~MovableUnit();
        bool enemy() const;
        void spawn();
        bool active() const;
        void move(int x, int y);
        MovableUnit* clone();
        void draw_self(ijengine::Canvas*, unsigned, unsigned);
        void draw_self_after(ijengine::Canvas*, unsigned, unsigned);
        void update_self(unsigned, unsigned);
        std::vector< std::pair<int, int> > m_labyrinth_path;
        int hp_percentage() const;
        std::string texture_name;

    protected:
        bool on_event(const ijengine::GameEvent& event);

    private:
        void die();
        bool m_enemy;
        std::pair<int, int> end_position;
        std::pair<int, int> start_position;
        std::pair<int, int> grid_position;
        std::shared_ptr<ijengine::Texture> m_texture;
        std::pair<int, int> desired_place;
        bool m_active;
        bool m_moving = false;
        int m_x;
        int m_y;
        unsigned int m_current_instruction;
        Player *m_player;
        int m_initial_hp = 100;
        int m_actual_hp = 100;
        Animation *m_animation;
        int m_total_states = 1;
        int m_frame_per_state = 1;
        Animation::StateStyle m_state_style;
    };
}

#endif

