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
}
