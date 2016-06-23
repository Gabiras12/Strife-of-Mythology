#ifndef SOMTD_PLAYER_H
#define SOMTD_PLAYER_H

#include <ijengine/game_object.h>

namespace SoMTD {
    class Player {
    public:
        enum PlayerState {
            IDLE = 0,
            HOLDING_BUILD = 1,
            INVALID_BUILD = 3,
            NOT_ENOUGH_GOLD = 4,
            SELECTED_TOWER = 5
        };

        void update_gold(int new_gold_count);
        Player();
        ~Player();
        int gold() const;
        int m_y;
        int m_x;
        int state;
        int m_hp = 50;
        int desired_tower = 0;
        ijengine::GameObject *selected_object = nullptr;

        void discount_gold(int);
    private:
        int m_gold;

    };
}

#endif
