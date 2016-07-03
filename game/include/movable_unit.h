#ifndef SOMTD_MOVABLE_UNIT_H
#define SOMTD_MOVABLE_UNIT_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/texture.h>
#include <string>
#include <memory>
#include <queue>
#include <list>
#include <set>
#include <vector>
#include "player.h"
#include "animation.h"

namespace SoMTD {

    class MovableUnit : public ijengine::GameObject, public ijengine::GameEventsListener {
    public:
        enum Status {
            NORMAL = 0x0000,
            SLOWED = 0x0001,
            TOTAL = 0x0002
        };

        MovableUnit(std::pair<int, int> s_pos, std::pair<int, int> e_pos, std::string texture_path, std::vector< std::pair<int, int> >, Player* playerz, Animation::StateStyle entity_state, int frame_per_state, int total_states, int unit_hp, int unit_reward, int time_per_tiles, int hp_discount_unit_win);
        ~MovableUnit();
        bool enemy() const;
        void spawn();
        bool active() const;
        void move(int x, int y, unsigned now);
        MovableUnit* clone();
        void draw_self(ijengine::Canvas*, unsigned, unsigned);
        void draw_self_after(ijengine::Canvas*, unsigned, unsigned);
        void update_self(unsigned, unsigned);
        std::vector< std::pair<int, int> > m_labyrinth_path;
        int hp_percentage() const;
        std::string texture_name;
        double x() const;
        double y() const;
        bool done() const;
        std::pair<int, int> start_position;
        Animation* animation() const;
        void suffer(int dmg);
        bool dead() const;
        int gold_award() const;
        int time_per_tile() const;
        void suffer_slow(int slow_coeff, int time_penalization, unsigned now, unsigned last);
        std::list<MovableUnit::Status> *status_list() const;

    protected:
        bool on_event(const ijengine::GameEvent& event);

    private:
        unsigned m_next_frame = 0;
        void die();
        bool m_done = false;
        bool m_enemy;
        std::pair<int, int> end_position;
        std::pair<int, int> grid_position;
        std::shared_ptr<ijengine::Texture> m_texture;
        std::pair<int, int> desired_place;
        bool m_active;
        bool m_moving = false;
        double m_x;
        double m_y;
        unsigned int m_current_instruction;
        std::pair<double, double> m_movement_speed;
        Player *m_player;
        int m_initial_hp = 100;
        int m_actual_hp = 100;
        int m_hp_discount_unit_win;
        Animation *m_animation;
        int m_total_states = 1;
        int m_frame_per_state = 1;
        Animation::StateStyle m_state_style;
        bool m_dead = false;
        int m_gold_award;
        int m_time_per_tile;
        std::list<MovableUnit::Status>* m_status_list;
        int m_slow_penalization;
        int m_slow_coeff;
        int m_hp_discount_unit;
    };
}

#endif

