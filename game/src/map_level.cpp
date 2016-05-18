#include <vector>

#include "game.h"

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/rectangle.h>

#include <fstream>
#include <cstring>

#include <SDL2/SDL.h>

#include "map_level.h"
#include "level_area.h"
#include "tower.h"

SoMTD::MapLevel::MapLevel(const string& next_level, const string& current_level) :
    m_done(false),
    m_next(next_level),
    m_start(-1),
    m_texture(nullptr),
    m_current(current_level)
{
    memset(grid, 0, sizeof grid);
    ijengine::event::register_listener(this);

    load_config_from_file();
    load_tiles();
    load_hud();
}

SoMTD::MapLevel::~MapLevel()
{
    ijengine::event::unregister_listener(this);
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
                    add_children(new SoMTD::LevelArea("waterfallEndE.png", 5, j, i));
                break;

                case 6:
                    add_children(new SoMTD::LevelArea("waterfallEndN.png", 6, j, i));
                break;

                case 7:
                    add_children(new SoMTD::LevelArea("waterfallEndW.png", 7, j, i));
                break;

                case 8:
                    add_children(new SoMTD::LevelArea("waterfallEndS.png", 8, j, i));
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
}

void
SoMTD::MapLevel::draw_self(ijengine::Canvas *canvas, unsigned, unsigned)
{
    canvas->clear();
    std::shared_ptr< ijengine::Texture > hud_texture = ijengine::resources::get_texture("hud.png");
    canvas->draw(hud_texture.get(), 0, 480-hud_texture.get()->h());
}

std::pair<int, int>
SoMTD::MapLevel::screen_coordinates(int map_x, int map_y, int tw, int th)
{
    int xs = (map_x - map_y) * (tw / 2);
    int ys = (map_x + map_y) * (th / 2);

    return std::pair<int, int>(xs, ys);
}

bool
SoMTD::MapLevel::on_event(const ijengine::GameEvent& event)
{
    if (event.type() == 0x04) {
        add_children(new SoMTD::LevelArea("tower_42.png", 9, m_children.size()-50, 3));
        return true;
    } else if (event.type() == 0x08) {
        m_done = true;
        return true;
    }
    return false;
}

void
SoMTD::MapLevel::load_hud()
{
}

