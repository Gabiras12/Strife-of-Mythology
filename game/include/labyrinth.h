#ifndef SOMTD_LABYRINTH_H
#define SOMTD_LABYRINTH_H

#include <vector>
#include <string>

namespace SoMTD {
    class Labyrinth {
    public:
        Labyrinth(int _columns, int _lines);
        ~Labyrinth();
        void fetch_file(std::string _path);
        std::vector<std::vector<int> > m_grid;

    private:
        int m_columns;
        int m_lines;
        void reset_grid(int _columns, int _lines);
        std::vector< std::pair<int, int> > breadth_first_search(std::pair<int, int> origin, std::pair<int, int> destination);

    };
}
#endif
