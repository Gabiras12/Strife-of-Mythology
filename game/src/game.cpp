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
    }

    void
    Game::load_keyboard_translations()
    {
        ijengine::KeyboardEvent a_button_event(1, ijengine::KeyboardEvent::PRESSED, ijengine::KeyboardEvent::A, ijengine::KeyboardEvent::NONE);
        ijengine::GameEvent ge(16);
        m_translator.translate(ge, a_button_event);
        //
        // ijengine::KeyboardEvent c_button_event(0, ijengine::KeyboardEvent::PRESSED, ijengine::KeyboardEvent::C, ijengine::KeyboardEvent::NONE);
        // ijengine::GameEvent ga(0x08);
        // m_translator.translate(ge, c_button_event);

        // ijengine::KeyboardEvent c_button_event(0, ijengine::KeyboardEvent::PRESSED, ijengine::KeyboardEvent::C, ijengine::KeyboardEvent::NONE);
        // m_translator.translate(ijengine::GameEvent(0x08), &c_button_event);
        //
        // ijengine::KeyboardEvent right_button_event(0, ijengine::KeyboardEvent::PRESSED, ijengine::KeyboardEvent::RIGHT, ijengine::KeyboardEvent::NONE);
        // m_translator.translate(ijengine::GameEvent(16), &right_button_event);
        //
        // ijengine::KeyboardEvent left_button_event(0, ijengine::KeyboardEvent::PRESSED, ijengine::KeyboardEvent::LEFT, ijengine::KeyboardEvent::NONE);
        // m_translator.translate(ijengine::GameEvent(32), &left_button_event);
        //
        // ijengine::KeyboardEvent up_button_event(0, ijengine::KeyboardEvent::PRESSED, ijengine::KeyboardEvent::UP, ijengine::KeyboardEvent::NONE);
        // m_translator.translate(ijengine::GameEvent(64), &up_button_event);
        //
        // ijengine::KeyboardEvent down_button_event(0, ijengine::KeyboardEvent::PRESSED, ijengine::KeyboardEvent::DOWN, ijengine::KeyboardEvent::NONE);
        // m_translator.translate(ijengine::GameEvent(128), &down_button_event);
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
