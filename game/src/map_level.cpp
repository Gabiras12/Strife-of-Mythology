#include "map_level.h"
#include <vector>

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/rectangle.h>

#include <fstream>
#include <cstring>

SoMTD::MapLevel::MapLevel(const string& next_level, const string& current_level) :
    m_done(false),
    m_next(next_level),
    m_start(-1),
    m_texture(nullptr),
    m_current(current_level)
{
    memset(grid, 0, sizeof grid);

    load_config_from_file();
}

void
SoMTD::MapLevel::load_config_from_file()
{
    if (not m_current.empty()) {
        std::string path("res/");
        path = path.append(m_current);
        path = path.append(".txt");

        std::ifstream map_data(path);
        if (map_data.is_open()) {
            // 30 is the expected number of tiles per rows and columns
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

    int x0 = 640/2;
    int y0 = 0;
    int xs;
    int ys;
    std::pair<int, int> p;
    for (int i=0; i < 8; ++i) {
        for (int j=0; j < 11; ++j) {
            switch (grid[i][j]) {
                case 1:
                    m_texture = ijengine::resources::get_texture("slopeE.png");
                    p = screen_coordinates(j, i, m_texture->w(), m_texture->h());
                    xs = p.first;
                    ys = p.second;
                    canvas->draw(m_texture.get(), xs + x0 - m_texture->w()/2, ys+y0 );
                break;

                case 2:
                    m_texture = ijengine::resources::get_texture("slopeN.png");
                    p = screen_coordinates(j, i, m_texture->w(), m_texture->h());
                    xs = p.first;
                    ys = p.second;
                    canvas->draw(m_texture.get(), xs + x0 - m_texture->w()/2, ys+y0 );
                break;

                case 3:
                    m_texture = ijengine::resources::get_texture("slopeW.png");
                    p = screen_coordinates(j, i, m_texture->w(), m_texture->h());
                    xs = p.first;
                    ys = p.second;
                    canvas->draw(m_texture.get(), xs + x0 - m_texture->w()/2, ys+y0 );
                break;

                case 4:
                    m_texture = ijengine::resources::get_texture("slopeS.png");
                    p = screen_coordinates(j, i, m_texture->w(), m_texture->h());
                    xs = p.first;
                    ys = p.second;
                    canvas->draw(m_texture.get(), xs + x0 - m_texture->w()/2, ys+y0 );
                break;

                case 5:
                    m_texture = ijengine::resources::get_texture("waterfallEndE.png");
                    p = screen_coordinates(j, i, m_texture->w(), m_texture->h());
                    xs = p.first;
                    ys = p.second;
                    canvas->draw(m_texture.get(), xs + x0 - m_texture->w()/2, ys+y0 );
                break;

                case 6:
                    m_texture = ijengine::resources::get_texture("waterfallEndN.png");
                    p = screen_coordinates(j, i, m_texture->w(), m_texture->h());
                    xs = p.first;
                    ys = p.second;
                    canvas->draw(m_texture.get(), xs + x0 - m_texture->w()/2, ys+y0 );
                break;

                case 7:
                    m_texture = ijengine::resources::get_texture("waterfallEndW.png");
                    p = screen_coordinates(j, i, m_texture->w(), m_texture->h());
                    xs = p.first;
                    ys = p.second;
                    canvas->draw(m_texture.get(), xs + x0 - m_texture->w()/2, ys+y0 );

                break;

                case 8:
                    m_texture = ijengine::resources::get_texture("waterfallEndS.png");
                    p = screen_coordinates(j, i, m_texture->w(), m_texture->h());
                    xs = p.first;
                    ys = p.second;
                    canvas->draw(m_texture.get(), xs + x0 - m_texture->w()/2, ys+y0 );
                break;



                default:
                break;

            }
        }
    }
}

std::pair<int, int>
SoMTD::MapLevel::screen_coordinates(int map_x, int map_y, int tw, int th)
{
    int xs = (map_x - map_y) * (tw / 2);
    int ys = (map_x + map_y) * (th / 2);

    return std::pair<int, int>(xs, ys);
}

