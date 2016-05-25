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
        // m_translator.translate(ijengine::SystemEvent(0, ijengine::SystemEvent::QUIT), ijengine::game_event::QUIT);

        load_keyboard_translations();
        ijengine::event::register_translator(&m_translator);
        ijengine::level::register_factory(&m_level_factory);
        ijengine::resources::set_textures_dir("res");
        ijengine::resources::set_fonts_dir("res");
    }

    void
    Game::load_keyboard_translations()
    {
        ijengine::KeyboardEvent a_button_event(1, ijengine::KeyboardEvent::PRESSED, ijengine::KeyboardEvent::A, ijengine::KeyboardEvent::NONE);
        ijengine::GameEvent ge(16);
        m_translator.translate(ge, a_button_event);
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
