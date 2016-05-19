#ifndef SOMTD_PLAYER_H
#define SOMTD_PLAYER_H

namespace SoMTD {
    class Player {
    public:
        Player();
        ~Player();
        int gold() const;
        int m_gold;
        int m_y;
        int m_x;
    };
}

#endif
