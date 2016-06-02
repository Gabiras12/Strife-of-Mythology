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
    tools::grid_to_isometric(int x_grid, int y_grid, int tile_width, int tile_height, int x0, int offset)
    {
        double y0 = tile_height/2.0;
        double xs = (x_grid - y_grid) * (tile_width/2) + x0;
        double ys = (x_grid + y_grid) * (-offset +tile_height/2.0) + y0;

        return std::pair<int, int>((int)xs, (int)ys);
    }

    std::pair<int, int>
    tools::isometric_to_grid(int xs, int ys, int w, int h, int x0, int offset)
    {
        x0 += w/2;
        double y0 = h/2;
        double xg = ((double)(xs - x0)/w) + ((double)ys-y0)/(h - 2.0*offset);
        double yg = (-(double)(xs-x0)/w) + (ys-y0)/(h - 2.0*offset);
        return std::pair<int, int>(xg, yg);
    }

}
