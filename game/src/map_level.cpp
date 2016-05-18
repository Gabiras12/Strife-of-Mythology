#include <vector>

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/rectangle.h>

#include <fstream>
#include <cstring>

#include <SDL2/SDL.h>

#include "map_level.h"
#include "level_area.h"

SoMTD::MapLevel::MapLevel(const string& next_level, const string& current_level) :
    m_done(false),
    m_next(next_level),
    m_start(-1),
    m_texture(nullptr),
    m_current(current_level)
{
    memset(grid, 0, sizeof grid);

    load_config_from_file();
    load_tiles();
}

void
SoMTD::MapLevel::load_tiles()
{
    for (int i=0; i < 9; ++i) {
        for (int j=0; j < 12; ++j) {
            switch (grid[i][j]) {
                case 1:
                    add_children(new SoMTD::LevelArea("slopeE.png", 1, j, i));
                break;

                case 2:
                    add_children(new SoMTD::LevelArea("slopeN.png", 2, j, i));
                break;

                case 3:
                    add_children(new SoMTD::LevelArea("slopeW.png", 3, j, i));
                break;

                case 4:
                    add_children(new SoMTD::LevelArea("slopeS.png", 4, j, i));
                break;

                case 5:
                    add_children(new SoMTD::LevelArea("waterfallEndE.png", 4, j, i));
                break;

                case 6:
                    add_children(new SoMTD::LevelArea("waterfallEndN.png", 4, j, i));
                break;

                case 7:
                    add_children(new SoMTD::LevelArea("waterfallEndW.png", 4, j, i));
                break;

                case 8:
                    add_children(new SoMTD::LevelArea("waterfallEndS.png", 4, j, i));
                break;

                default:
                break;
            }
        }
    }
}

void
SoMTD::MapLevel::load_config_from_file()
{
    // fetch data from a file with its level id
    if (not m_current.empty()) {
        std::string path("res/");
        path = path.append(m_current);
        path = path.append(".txt");

        std::ifstream map_data(path);
        if (map_data.is_open()) {
            // 9 = number of rows
            // 12 = number of cols
            for (int i=0; i < 9; ++i) {
                for (int j=0; j < 12; ++j) {
                    map_data >> grid[i][j];
                }
            }
            map_data.close();
        }
    }
}


bool
SoMTD::MapLevel::done() const
{
    return m_done;
}

std::string
SoMTD::MapLevel::next() const
{
    return m_next;
}

void
SoMTD::MapLevel::update_self(unsigned now, unsigned)
{
    if (m_start == -1)
        m_start = now;
    if (now - m_start > 5000)
        m_done = true;
}

void
SoMTD::MapLevel::draw_self(ijengine::Canvas *canvas, unsigned, unsigned)
{
    canvas->clear();
}

std::pair<int, int>
SoMTD::MapLevel::screen_coordinates(int map_x, int map_y, int tw, int th)
{
    int xs = (map_x - map_y) * (tw / 2);
    int ys = (map_x + map_y) * (th / 2);

    return std::pair<int, int>(xs, ys);
}

