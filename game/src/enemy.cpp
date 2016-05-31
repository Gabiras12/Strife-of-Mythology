#include <iostream>

#include "enemy.h"

SoMTD::Enemy::Enemy()
{
    m_x_grid = 0;
    m_y_grid = 0;
    m_x = 0;
    m_y = 0;
}

SoMTD::Enemy::Enemy(int _x_grid, int _y_grid)
{

}

SoMTD::Enemy::~Enemy()
{
}

SoMTD::Enemy*
SoMTD::Enemy::clone()
{
    Enemy *e = new Enemy(m_x_grid, m_y_grid);
}
