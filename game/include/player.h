#ifndef SOMTD_PLAYER_H
#define SOMTD_PLAYER_H

#include <ijengine/game_object.h>
#include <list>

namespace SoMTD {
    class Player {

    public:
        enum PlayerState {
            IDLE = 0,
            HOLDING_BUILD = 1,
            INVALID_BUILD = 3,
            NOT_ENOUGH_GOLD = 4,
            SELECTED_TOWER = 5,
            OPENED_TOWER_PANEL = 6
        };

        Player();
        ~Player();
        int gold() const;
        int hp() const;
        int desired_tower() const;

        int state;
        ijengine::GameObject *selected_object = nullptr;

        void discount_gold(int);
        void update_gold(int new_gold_count);
        void discount_hp(int);
        void update_desired_tower(int);
        std::list<int> *units_events() const;
        std::list<int> *event_args() const;
        void open_tower_panel(unsigned tower_id);
        unsigned tower_panel_id() const;

    private:
        int m_gold;
        int m_y;
        int m_x;
        int m_hp = 50;
        int m_desired_tower = 0;
        std::list<int> *m_units_events;
        std::list<int> *m_event_args;
        bool m_buy_tower_panel_opened;
        unsigned m_tower_panel_id;
    };
}

#endif
