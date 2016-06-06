#include "player.h"

SoMTD::Player::Player() :
    m_gold(9000),
    m_y(0),
    m_x(0),
    state(0x0000)
{
}

SoMTD::Player::~Player()
{

}

int
SoMTD::Player::gold() const
{
	return m_gold;
}

void 
SoMTD::Player::update_gold(int new_gold_count)
{
	m_gold = new_gold_count;
}

void
SoMTD::Player::discount_gold(int value)
{
	m_gold -= value;
}