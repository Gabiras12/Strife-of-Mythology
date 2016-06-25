#include "player.h"

SoMTD::Player::Player() :
    m_y(0),
    m_x(0),
    state(0x0000)
{
    m_gold = 9000;
}

SoMTD::Player::~Player()
{

}

int
SoMTD::Player::gold() const
{
	return m_gold;
}

int
SoMTD::Player::hp() const
{
	return m_hp;
}

int
SoMTD::Player::desired_tower() const
{
	return m_desired_tower;
}

void
SoMTD::Player::update_desired_tower(int m_id){
  m_desired_tower = m_id - 4;
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

void
SoMTD::Player::discount_hp(int value)
{
	m_hp -= value;
}
