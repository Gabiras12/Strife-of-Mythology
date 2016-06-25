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

    private:
        int m_gold;
        int m_y;
        int m_x;
        int m_hp = 50;
        int m_desired_tower = 0;



    };
}

#endif
