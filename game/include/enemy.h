#ifndef SOMTD_ENEMY_H
#define SOMTD_ENEMY_H

#include "movable_unit_prototype.h"
#include <ijengine/texture.h>

namespace SoMTD {
    class Enemy : public MovableUnitPrototype {
    public:
        Enemy(int _x_grid, int _y_grid);
        Enemy();
        ~Enemy();
        Enemy* clone();

    private:
        ijengine::Texture *m_texture;
        int m_x_grid; // tiles
        int m_y_grid;
        int m_x; // pixels
        int m_y;
    };
}

#endif
