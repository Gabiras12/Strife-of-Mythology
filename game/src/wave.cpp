#include <fstream>
#include <vector>
#include "wave.h"
#include "movable_unit.h"
#include <iostream>

SoMTD::Wave::Wave(unsigned p_id) :
    m_done(false),
    m_started(false),
    m_id(p_id),
    m_spawning(false),
    m_units(new std::list<SoMTD::MovableUnit*>())
{
}

SoMTD::Wave::~Wave()
{
    delete m_units;
}

unsigned
SoMTD::Wave::id() const
{
    return m_id;
}

std::vector<int>
SoMTD::Wave::units_idx() const
{
    return m_units_idx;
}

void
SoMTD::Wave::add_unit(SoMTD::MovableUnit *u)
{
    m_units->push_back(u);
}

void
SoMTD::Wave::spawn_unit()
{
    if (current_unit_it() == units()->end()) {
        m_spawning = false;
    } else {
        if (current_unit()) {
            current_unit()->spawn();
            m_current_unit_it++;
        }
    }
}

bool
SoMTD::Wave::done() const
{
    return m_done;
}

void
SoMTD::Wave::update_self(unsigned now, unsigned a2)
{
    if (started()) {
        int i = 0;
        for (auto it=units()->begin(); it != units()->end(); ++it) {
            if ((*it)->done() == true) {
                i++;
            }
        }
        if (i == units()->size()) {
            finish();
        }
        for (auto it=units()->begin(); it != units()->end(); ++it) {
            if ((*it)->done() == true) {
                it = units()->erase(it);
            } else {
                (*it)->update_self(now, a2);
            }
            ++i;
        }
    }

    if (spawning()) {
        if (m_last_spawned_unit_time+300 < now) {
            m_last_spawned_unit_time = now;
            spawn_unit();
        }
    }
}

void
SoMTD::Wave::finish()
{
    m_done = true;
}

SoMTD::MovableUnit*
SoMTD::Wave::current_unit() const
{
    return *m_current_unit_it;
}

std::list<SoMTD::MovableUnit*>::iterator
SoMTD::Wave::current_unit_it() const
{
    return m_current_unit_it;
}

bool
SoMTD::Wave::started() const
{
    return m_started;
}

void
SoMTD::Wave::start(unsigned now)
{
    m_current_unit_it = units()->begin();
    m_started_at = now;
    m_started = true;
    m_spawning = true;
    m_last_spawned_unit_time = now;
}

unsigned
SoMTD::Wave::started_at() const
{
    return m_started_at;
}

bool
SoMTD::Wave::spawning() const
{
    return m_spawning;
}

std::list<SoMTD::MovableUnit*>*
SoMTD::Wave::units() const
{
    return m_units;
}

void
SoMTD::Wave::draw_self(ijengine::Canvas *c, unsigned a1, unsigned a2)
{
    for (auto it : *units()) {
        if (it->active()) {
            it->draw_self(c, a1, a2);
        }
    }
}

void
SoMTD::Wave::draw_self_after(ijengine::Canvas *c, unsigned a1, unsigned a2)
{
    for (auto it : *units()) {
        if (it->active()) {
            it->draw_self_after(c, a1, a2);
        }
    }
}
