#ifndef SOMTD_LABYRINTH_H
#define SOMTD_LABYRINTH_H

#include <vector>
#include <string>

namespace SoMTD {
    class Labyrinth {
    public:
        Labyrinth(int _columns, int _lines, std::pair<int, int>myorigin, std::pair<int, int>mydestiny);
        ~Labyrinth();
        void fetch_file(std::string _path);
        std::vector<std::vector<int> > m_grid;
        std::vector< std::pair<int, int> > solve();

        void update_origin(std::pair<int, int> new_origin) {
            m_origin = new_origin;
        }

        void update_destiny(std::pair<int, int> new_destiny) {
            m_destiny = new_destiny;
        }

        std::vector< std::pair<int, int> > solution;

    private:
        int m_columns;
        int m_lines;
        void reset_grid(int _columns, int _lines);
        std::pair<int, int> m_origin;
        std::pair<int, int> m_destiny;
    };
}
#endif
