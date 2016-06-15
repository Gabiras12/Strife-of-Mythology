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
        void add_unit(int unit_id);
        bool started() const;
        void update_self(unsigned start, unsigned end);
        void start();

    private:
        unsigned m_id;
        bool m_done;
        std::vector<int> m_units;
        void fetch_file(std::string);
        bool m_started;
    };
}

#endif
