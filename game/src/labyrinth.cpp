#include "labyrinth.h"
#include <fstream>
#include <queue>
#include <algorithm>

SoMTD::Labyrinth::Labyrinth(int _columns, int _lines, std::pair<int, int> neworigin, std::pair<int, int> destiny) :
    m_columns(_columns),
    m_lines(_lines),
    m_origin(neworigin),
    m_destiny(destiny)
{
    reset_grid(_columns, _lines);
}

SoMTD::Labyrinth::~Labyrinth()
{

}

void
SoMTD::Labyrinth::reset_grid(int _columns, int _lines)
{
    m_grid.resize(_columns);
    for (int i=0; i < _columns; ++i) {
        m_grid[i].clear();
        m_grid[i].resize(_lines);
    }
}

void
SoMTD::Labyrinth::fetch_file(std::string path)
{
    std::ifstream map_data(path);
    if (map_data.is_open()) {
        for (int i=0; i < m_lines; ++i) {
            for (int j=0; j < m_columns; ++j) {
                map_data >> std::hex >> m_grid[i][j];
            }
        }
        map_data.close();
    }
}

std::vector< std::pair<int, int> >
SoMTD::Labyrinth::solve()
{
    printf("solving...\n");
    std::vector< std::vector< std::pair<int, int> > > father(m_lines);
    std::vector< std::vector<int> > visited(m_lines);
    for (auto &it : father) {
        it.resize(m_columns);
    }

    for (auto &it : visited) {
        it.resize(m_columns);
    }

    for (int i=0; i < m_lines; ++i) {
        for (int j=0; j < m_columns; ++j) {
            father[i][j] = std::pair<int, int>(j, i);
            visited[i][j] = 0;
        }
    }

    std::vector< std::pair<int, int> > solution_path;

    visited[m_origin.second][m_origin.first] = 1;
    std::queue<std::pair<int, int> >myq;
    myq.push(m_origin);

    while (not myq.empty()) {
        std::pair<int, int> pos = myq.front();

        if (pos == m_destiny) {
            while (father[pos.second][pos.first] != std::pair<int, int>(pos.first, pos.second)) {
                solution_path.push_back(pos);
                pos = father[pos.second][pos.first];
            }
            solution_path.push_back(pos);
            break;
        }

        std::vector< std::pair<int, int> > myv { std::make_pair(pos.first+1, pos.second), std::make_pair(pos.first-1, pos.second), std::make_pair(pos.first, pos.second+1), std::make_pair(pos.first, pos.second-1) };
        for (auto adj : myv) {
            if (adj.first >= 0 && adj.second >= 0 && adj.first < m_columns && adj.second < m_lines) {
                if (m_grid[adj.second][adj.first] != 0 && m_grid[adj.second][adj.first] != 10 && m_grid[adj.second][adj.first] != 0x7 && m_grid[adj.second][adj.first] != 0x11 && m_grid[adj.second][adj.first] != 20 && m_grid[adj.second][adj.first] != 27) {
                    if (visited[adj.second][adj.first] == 0) {
                        visited[adj.second][adj.first] = 1;
                        father[adj.second][adj.first] = pos;
                        myq.push(adj);
                    }
                }
            }
        }
        myq.pop();
    }

    solution = solution_path;
    return solution_path;
}
