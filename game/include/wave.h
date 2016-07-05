#ifndef SOMTD_WAVE_H
#define WAVE_H

#include <vector>
#include <list>
#include "movable_unit.h"

namespace SoMTD {
    class Wave {
    public:
        Wave(unsigned p_id);
        ~Wave();
        unsigned id() const;
        bool done() const;
        std::vector<int> units_idx() const;
        bool started() const;
        void start(unsigned now);
        unsigned started_at() const;
        bool spawning() const;
        std::list<MovableUnit*>* units() const;
        std::list<MovableUnit*>::iterator current_unit_it() const;

        void update_self(unsigned start, unsigned end);
        void add_unit(SoMTD::MovableUnit* u);
        SoMTD::MovableUnit *current_unit() const;
        void spawn_unit();
        void finish();
        void draw_self(ijengine::Canvas *c, unsigned a1, unsigned a2);
        void draw_self_after(ijengine::Canvas *c, unsigned a1, unsigned a2);

    private:
        unsigned m_started_at;
        bool m_done;
        std::vector<int> m_units_idx;
        void fetch_file(std::string);
        bool m_started;
        unsigned m_id;
        std::list<MovableUnit*>::iterator m_current_unit_it;
        int m_current_unit_idx;
        bool m_spawning;
        std::list<MovableUnit*> *m_units;
        unsigned m_last_spawned_unit_time;
    };
}

#endif
