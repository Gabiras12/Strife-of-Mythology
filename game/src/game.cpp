#include "game.h"

#include <ijengine/system_event.h>

namespace SoMTD {

    Game::Game(const string& title, int w, int h) :
        m_game(title, w, h),
        m_engine(),
        m_level_factory()
    {
        m_translator.add_translation(ijengine::SystemEvent(0, ijengine::SystemEvent::QUIT), ijengine::GameEvent(GAME_EVENT_QUIT));
        ijengine::event::register_translator(&m_translator);
        ijengine::level::register_factory(&m_level_factory);

        ijengine::resources::set_textures_dir("res");
    }

    Game::~Game()
    {
        ijengine::level::unregister_factory();
        ijengine::event::unregister_translator(&m_translator);
    }

    int
    Game::run(const string& level_id)
    {
        printf("running level %s\n", level_id.c_str());
        return m_game.run(level_id);
    }
}
