#include "labyrinth.h"
#include <fstream>

SoMTD::Labyrinth::Labyrinth(int _columns, int _lines) :
    m_columns(_columns),
    m_lines(_lines)
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
                map_data >> m_grid[i][j];
            }
        }
        map_data.close();
    }
}

std::vector< std::pair<int, int> >
SoMTD::Labyrinth::breadth_first_search(std::pair<int, int>, std::pair<int, int>)
{
    // std::queue<std::pair<int, int> >myq;
    // myq.push(std::make_pair(origin.first, origin.second));
    // std::vector< std::vector<int> > visited;
    // visited.resize(m_line);
    // for (auto it : visited) {
    //     it.resize(m_column);
    // }
    // int visited[30][30];
    // std::pair<int, int> father[30][30];
    // for (int i=0; i < 30; ++i) {
    //     for (int j=0; j < 30; ++j) {
    //         father[i][j] = std::pair<int, int>(j, i);
    //     }
    // }
    // visited[origin.second][origin.first] = 1;
    // memset(visited, 0, sizeof visited);
    // std::vector< std::pair<int, int> > path;
    // while (not myq.empty()) {
    //     int x = myq.front().first;
    //     int y = myq.front().second;
    //
    //     visited[y][x] = 1;
    //
    //     if (x == destiny.first && y == destiny.second) {
    //         int aux_x = x;
    //         int aux_y = y;
    //         while (father[aux_y][aux_x] != std::pair<int, int>(aux_x, aux_y)) {
    //             path.push_back( std::pair<int, int>(aux_x, aux_y));
    //             std::pair<int, int> idx = father[aux_y][aux_x];
    //             aux_x = idx.first;
    //             aux_y = idx.second;
    //         }
    //
    //         return path;
    //     }
    //
    //     std::vector< std::pair<int, int> > myv { std::make_pair(x+1, y), std::make_pair(x-1, y), std::make_pair(x, y+1), std::make_pair(x, y-1) };
    //     for (auto it : myv) {
    //         if (it.first >= 0 && it.first < 10 && it.second >= 0 && it.second < 10) {
    //             if (m_labyrinth->m_grid[it.second][it.first] != 6) {
    //                 if (not visited[it.second][it.first]) {
    //                     myq.push(std::make_pair(it.first, it.second));
    //                     visited[it.second][it.first] = 1;
    //                     father[it.second][it.first] = std::pair<int, int>(x, y);
    //                 }
    //             }
    //         }
    //     }
    //
    //     myq.pop();
    // }
    // return path;
    std::vector< std::pair<int, int> > p;
    return p;
}
