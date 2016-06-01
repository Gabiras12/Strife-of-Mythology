#include "game.h"
#include <vector>

#include <ijengine/system_event.h>
#include <ijengine/keyboard_event.h>

namespace SoMTD {
    Game::Game(const string& title, int w, int h) :
        m_game(title, w, h),
        m_engine(),
        m_level_factory()
    {
        ijengine::event::register_translator(&m_translator);
        ijengine::level::register_factory(&m_level_factory);
        ijengine::resources::set_textures_dir("res");
        ijengine::resources::set_fonts_dir("res");
    }

    Game::~Game()
    {
        ijengine::level::unregister_factory();
        ijengine::event::unregister_translator(&m_translator);
    }

    int
    Game::run(const string& level_id)
    {
        return m_game.run(level_id);
    }

    std::pair<int, int>
    tools::grid_to_isometric_canvas(int x_grid, int y_grid, int tile_width, int tile_height)
    {
        int xs = (x_grid - y_grid) * (tile_width/2);
        int ys = (x_grid + y_grid) * (tile_height/2);

        return std::pair<int, int>(xs, ys);
    }

    std::pair<int, int>
    tools::isometric_adjust(int isometric_x, int isometric_y, int x0, int tile_offset, int tile_width, int tile_height, int grid_x, int grid_y)
    {
        std::pair<int, int> p;
        p.first = isometric_x + x0 - tile_width/2;
        p.second = isometric_y - (tile_offset*(grid_x + grid_y));
        return p;
    }

    std::pair<int, int>
    tools::isometric_to_grid(int isometric_x, int isometric_y, int tile_width, int tile_height, int x0, int offset)
    {
        int h_tw = tile_width/2;
        int h_th = tile_height/2;

        std::pair<int, int> p;
        p.first = (((isometric_x+h_th-x0)/h_tw)+((isometric_y)/(h_th-offset)))/2;
        p.second = (((isometric_y)/(h_th-offset)) - ((isometric_x+h_tw-x0)/h_tw))/2;
        return p;
    }

}
