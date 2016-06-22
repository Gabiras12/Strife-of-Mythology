#ifndef SOMTD_WAVE_H
#define WAVE_H

#include <vector>

namespace SoMTD {
    class Wave {
    public:
        Wave(unsigned p_id);
        ~Wave();
        unsigned id() const;
        bool done() const;
        std::vector<int> units() const;
        bool started() const;
        void start(unsigned now);
        unsigned started_at() const;
        bool spawning() const;

        void update_self(unsigned start, unsigned end);
        void add_unit(int unit_id);
        int current_unit();
        void spawn_unit();
        void finish();

    private:
        unsigned m_id;
        unsigned m_started_at;
        bool m_done;
        std::vector<int> m_units;
        void fetch_file(std::string);
        bool m_started;
        int m_current_unit_idx = 0;
        bool m_spawning;
    };
}

#endif
