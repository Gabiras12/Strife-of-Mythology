#include "player.h"
#include <ijengine/engine.h>

SoMTD::Player::Player() :
    m_y(0),
    m_x(0)
{
    state = 0x0000;
    m_gold = 1200;
    m_units_events = new std::list<int>();
    m_event_args = new std::list<int>();
    m_buy_tower_panel_opened = false;
    m_upgrade_state.reset();
    m_upgrade_state.set(0);
}

SoMTD::Player::~Player()
{
    delete m_units_events;
    delete m_event_args;
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
SoMTD::Player::update_desired_tower(int m_id, int price){
  m_desired_tower = m_id;
  m_desired_tower_price = price;
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
    ijengine::audio::play_sound_effect("res/sound_efects/gold.ogg");
}

void
SoMTD::Player::increase_gold(int value)
{
	m_gold += value;
}

void
SoMTD::Player::discount_hp(int value)
{
	m_hp -= value;
}

std::list<int>*
SoMTD::Player::units_events() const
{
    return m_units_events;
}

std::list<int>*
SoMTD::Player::event_args() const
{
    return m_event_args;
}

void
SoMTD::Player::open_tower_panel(unsigned id)
{
    m_buy_tower_panel_opened = true;
    m_tower_panel_id = id;
}

unsigned
SoMTD::Player::tower_panel_id() const
{
    return m_tower_panel_id;
}

std::bitset<12>
SoMTD::Player::upgrade_state() const
{
    return m_upgrade_state;
}

void
SoMTD::Player::research(std::bitset<12> upgrade)
{
    m_upgrade_state |= upgrade;
}
